<?php
ini_set('display_errors', 'stderr');

/*
 * Class Parser()
 * Parser class that parses and sanitizes the output.
 * Creates Element objects.
 */
class Parser {
    public $parserXML;
    private $counter = 1;

    function __construct() {
        $this->parserXML = new SimpleXMLElement('<?xml version="1.0" encoding="utf-8"?><program></program>');
        $this->parserXML->addAttribute('language', 'IPPcode23');
    }

    function parse() {
        $this->get_head();

        while($line = fgets(STDIN)) {
            $line = explode("#", $line)[0];
            # Skips empty lines
            if($this->check_empty($line)) continue;

            # Checks if we found header again
            if (preg_match('/\..*/', $line)) exit(22);
            
            # Generates XML element
            $this->generate_element($line);
        }
    }
    /*
     * Method get_head()
     * Checks for header in input file
     * Exits with 21 if not found
     */
    function get_head() {
        do {
        $line = fgets(STDIN);
        $line = explode("#", $line)[0];
        } while ($this->check_empty($line));

        if (trim(strtolower($line)) != ".ippcode23") exit(21);
    }

    /*
     * Method check_empty($line)
     * Returns true if line is empty.
     */
    function check_empty($line) {
        if (
            empty($line) ||
            $line === "\n"
        ) { 
            return true; 
        }
        return false;
    }
    /*
     * Method generate_element(%line)
     * Generates new Element object from input line
     * Replaces special xml chars
     */
    function generate_element($line) {
        // removes extra spaces
        $line = preg_replace('/\s+/', ' ', $line);

        // special characters
        $line = preg_replace('/&/', '&amp;', $line);
        $line = preg_replace('/</', '&lt;', $line);
        $line = preg_replace('/>/', '&gt;', $line);
        
        $line = trim($line);
        $line = explode(" ", $line);
        $element = new Element($this->parserXML, $this->counter++, $line);
        $element->check_args($line);
    } 
    
}

/*
 * Class Element($xml, $counter, $line)
 * Class to create new element in $xml
 */
class Element {
    private $element;

    private $intReg = "/^int@[+-]?\d+$/";
    private $strReg = "/^string@(([^#\\\\@]+|\\\\[0-9]{3})*)$/";
    private $nilReg = "/^nil@nil$/";
    private $varReg = "/^(lf|gf|tf)@([^#\\\\@\d\/]([^#\\\\@]+|\\\\[0-9]{3})*)$/";
    private $boolReg = "/^bool@(true|false)$/";
    private $labelReg = "/^[_\-$&%*!?]?[\w(0-9)_\-$&%*!?]+$/";
    private $varStartReg = "/(LF|GF|TF)/";
    private $readReg = "/^(int|string|bool)$/";

    function __construct($xml, $counter, $line) {
        $this->element = $xml->addChild("instruction");
        $this->element->addAttribute('order', $counter++);
        $this->element->addAttribute('opcode', strtoupper(trim($line[0])));
    } 

    /*
     * Method check_args($line)
     * Checks opcode, if invalid exits with 22
     * Checks if the first param is correct
     * Adds arguments based on opcode
     */
    function check_args($line) {
        switch(strtolower($line[0])) {
        # all the ones with 3 args
        case "add":
        case "sub":
        case "mul":
        case "idiv":
        case "lt":
        case "gt":
        case "eq":
        case "and":
        case "or":
        case "stri2int":
        case "concat":
        case "getchar":
        case "setchar":
            if (count($line) !== 4) exit(23);
            $arg1 = $this->check_symb($line[1]);
            $arg2 = $this->check_symb($line[2]);
            $arg3 = $this->check_symb($line[3]);

            if($arg1 !== "var") exit(23); // First needs to be var

            $this->add_argument($arg1, $line[1], "arg1");
            $this->add_argument($arg2, $line[2], "arg2");
            $this->add_argument($arg3, $line[3], "arg3");

            break;    
        
        case "jumpifeq":
        case "jumpifneq":
            if (count($line) !== 4) exit(23);
            if (!preg_match($this->labelReg, strtolower($line[1]))) exit(23); // first is label

            $arg2 = $this->check_symb($line[2]);
            $arg3 = $this->check_symb($line[3]);

            $this->add_argument("label", $line[1], "arg1");
            $this->add_argument($arg2, $line[2], "arg2");
            $this->add_argument($arg3, $line[3], "arg3");
            break;

        # 2 args ones
        case "move":
        case "int2char":
        case "strlen":
        case "type":
        case "not":
            if (count($line) !== 3) exit(23);

            $arg1 = $this->check_symb($line[1]);
            $arg2 = $this->check_symb($line[2]);

            if($arg1 !== "var") exit(23); // First needs to be var

            $this->add_argument($arg1, $line[1], "arg1");
            $this->add_argument($arg2, $line[2], "arg2");
            break;

        case "read":
            if (count($line) !== 3) exit(23);
            
            $arg1 = $this->check_symb($line[1]);
            if ($arg1 !== "var") exit(23);

            if (!preg_match($this->readReg, $line[2])) exit(23);

            $this->add_argument($arg1, $line[1], "arg1");
            $this->add_argument("type", $line[2], "arg2");

            break;

        # 1 arg ones
        case "call":
        case "label":
        case "jump":
            if (count($line) !== 2) exit(23);
            if (!preg_match($this->labelReg, strtolower($line[1]))) exit(23); // first is label
            $this->add_argument("label", $line[1], "arg1");
            break;

        case "defvar":
        case "pops":
            if (count($line) !== 2) exit(23);
            $arg1 = $this->check_symb($line[1]);
    
            if ($arg1 !== "var") exit(23);

            $this->add_argument("var", $line[1], "arg1");
            break;

        case "exit":
        case "dprint":
        case "pushs":
        case "write":
            if (count($line) !== 2) exit(23);
            $arg1 = $this->check_symb($line[1]);
            $this->add_argument($arg1, $line[1], "arg1");
            break;

        # no arg ones
        case "createframe":
        case "pushframe":
        case "popframe":
        case "return":
        case "break":
            if (count($line) !== 1) exit(23);
            break;

        default:
            exit(22);
        }
    }

    /*
     * Method check_symb($symbol)
     * Checks symbol and returns its data type if valid
     * If not exits with 23
     */
    function check_symb($symbol) {
        // symbol can be const
        if (preg_match($this->intReg, $symbol)) return ("int");
        if (preg_match($this->strReg, strtolower($symbol))) return ("string");
        if (preg_match($this->nilReg, $symbol)) return ("nil");
        if (preg_match($this->boolReg, $symbol)) return ("bool");

        // var is weird, first part needs to be upper according to tests
        if (preg_match($this->varReg, strtolower($symbol))) {
            if (preg_match($this->varStartReg, explode("@", $symbol)[0])) {
            return("var");
            }
        } 
        
        exit(23);
    }

    /*
     * Method add_argument($type, $value, $arg)
     * Adds argument to current element
     */
    function add_argument($type, $value, $arg) {
        if ($type !== "var" && $type !== "label" && $type !== "type") {
            $value = explode("@", $value, 2)[1];
        }
        $argument = $this->element->addChild($arg, $value);
        $argument->addAttribute('type', $type);
    }
}

/*
 * Function print_help()
 * Echoes help when script is called with --help
 */
function print_help() {
    echo("Usage: php parse.php [--help]\n");
    echo("\n");
    echo("Reads IPPcode23 input file from STDIN\n");
    echo("\n");
    echo("--help\t\tprints this help\n");
}

/*
 * Function print_badArgsMsg()
 * Echoes message when bad args are passed
 */
function print_badArgsMsg() {
    echo("parse.php: unrecognized options\n");
    echo("Try 'parse.php --help' for more information\n");
}

if ($argc === 2) {
    if ($argv[1] === "--help") {
        print_help();
        exit(0);
    }
    else {
        print_badArgsMsg();
        exit(10);
    }
} else if ($argc > 2) {
    print_badArgsMsg();
    exit(10);
}

$parser = new Parser();

$parser->parse();

echo ($parser->parserXML->asXML());

exit(0);
?>
