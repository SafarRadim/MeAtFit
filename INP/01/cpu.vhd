-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpreter)
-- Copyright (C) 2022 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Radim SAFAR <xsafar27 AT stud.fit.vutbr.cz>
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK          : in std_logic;                         -- hodinovy signal
   RESET        : in std_logic;                         -- asynchronni reset procesoru
   EN           : in std_logic;                         -- povoleni cinnosti procesoru
 
   -- synchronni pamet RAM
   DATA_ADDR    : out std_logic_vector(12 downto 0);    -- adresa do pameti
   DATA_WDATA   : out std_logic_vector(7 downto 0);     -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA   : in  std_logic_vector(7 downto 0);     -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR    : out std_logic;                        -- cteni (0) / zapis (1)
   DATA_EN      : out std_logic;                        -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA      : in  std_logic_vector(7 downto 0);     -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD       : in  std_logic;                        -- data platna
   IN_REQ       : out std_logic;                        -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA     : out std_logic_vector(7 downto 0);     -- zapisovana data
   OUT_BUSY     : in  std_logic;                        -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE       : out std_logic                         -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

    signal PC_reg   : std_logic_vector(11 downto 0);                    -- Instruction Pointer
    signal PC_inc   : std_logic;
    signal PC_dec   : std_logic;

    signal PTR_reg  : std_logic_vector(11 downto 0);    -- Memory Pointer
    signal PTR_inc  : std_logic;
    signal PTR_dec  : std_logic;

    signal CNT_reg  : std_logic_vector(7 downto 0);    -- CNT

    type muxSel is (
        program, 
        data
    );

    signal mux_sel : muxSel;            -- Selector for address mux


    type FSM_state_type is (
        start,          -- Utility instructions
        finish,
        fetch,
        decode,

        decode_wait,    -- Wait helpers
        inc_wait,
        dec_wait,
        output_wait,
        input_wait,
        
        shiftRight,     -- Movement instructions
        shiftLeft,

        inc,            -- Increment and Decrement
        dec,

        whileBegin,     -- loops
        whileWaitBegin,
        whileSkip,
        whileSkipWait,
        whileEnd,
        whileWaitEnd,
        whileGoBack,
        whileGoBackWait,
        whileGoBackDelay,

        doWhileBegin,
        doWhileEnd,
        doWhileWaitEnd,
        doWhileGoBack,
        doWhileGoBackWait,
        doWhileGoBackDelay,

        output,         -- I/O
        input
    );

    signal FSM_state: FSM_state_type := start;

begin

    -- PC register
    pc: process (CLK, RESET)
    begin
        if (RESET = '1') then
            PC_reg <= x"000";
        elsif rising_edge(CLK) then
            if (PC_inc = '1') then
                PC_reg <= PC_reg + 1;
            elsif (PC_dec = '1') then
                PC_reg <= PC_reg -1;
            end if;
        end if;
    end process;

    -- PTR register
    ptr: process (CLK, RESET)
    begin
        if (RESET = '1') then
            PTR_reg <= x"000";
        elsif rising_edge(CLK) then
            if (PTR_inc = '1') then
                PTR_reg <= PTR_reg + 1;
            elsif (PTR_dec = '1') then
                PTR_reg <= PTR_reg - 1;
            end if;
        end if;
    end process;

    -- Addr muxer
    abMux: process (mux_sel, PC_reg, PTR_reg)
    begin
        if (mux_sel = program) then           -- Program
            DATA_ADDR <= '0' & PC_reg;
        elsif (mux_sel = data) then        -- Data
            DATA_ADDR <= '1' & PTR_reg;
        end if;
    end process;

    fsm: process(CLK, RESET)
    begin
        if RESET = '1' then
            FSM_state <= start;

            PC_inc <= '0';
            PC_dec <= '0';

            PTR_inc <= '0';
            PTR_dec <= '0';

            CNT_reg <= x"00";

            DATA_EN <= '0';                 -- TDD baby
            IN_REQ <= '0';
            OUT_WE <= '0';

        elsif rising_edge(CLK) and EN = '1' then
            case FSM_state is
                when start =>
                    DATA_en <= '0';
                    mux_sel <= program;
                    PC_inc <= '0';
                    OUT_WE <= '0';
                    FSM_state <= fetch;

                when fetch =>
                    DATA_RDWR <= '0';
                    DATA_EN <= '1';
                    FSM_state <= decode_wait;

                when decode_wait =>
                    FSM_state <= decode;

                when decode =>
                    DATA_EN <= '0';
                    case DATA_RDATA is
                        when x"3E" =>
                            PTR_inc <= '1';
                            FSM_state <= shiftRight;

                        when x"3C" =>
                            PTR_dec <= '1';
                            FSM_state <= shiftLeft;

                        when x"2B" =>
                            mux_sel <= data;
                            DATA_EN <= '1';
                            FSM_state <= inc;

                        when x"2D" =>
                            mux_sel <= data;
                            DATA_EN <= '1';
                            FSM_state <= dec;

                        when x"5B" =>
                            mux_sel <= data;
                            DATA_EN <= '1';
                            CNT_reg <= x"01";
                            FSM_state <= whileBegin;

                        when x"5D" =>
                            DATA_EN <= '1';
                            CNT_reg <= x"01";
                            mux_sel <= data;
                            FSM_state <= whileEnd;

                        when x"28" =>
                            FSM_state <= doWhileBegin;

                        when x"29" =>
                            DATA_EN <= '1';
                            mux_sel <= data;
                            CNT_reg <= x"01";
                            FSM_state <= doWhileEnd;

                        when x"2E" =>
                            DATA_EN <= '1';
                            mux_sel <= data;
                            FSM_state <= output_wait;

                        when x"2C" =>
                            mux_sel <= data;
                            IN_REQ <= '1';
                            FSM_state <= input_wait;

                        when x"00" =>
                            FSM_state <= finish;

                        when others =>
                            PC_inc <= '1';
                            FSM_state <= start;

                    end case;

                when shiftRight =>
                    PC_inc <= '1';
                    PTR_inc <= '0';
                    mux_sel <= data;
                    FSM_state <= start;

                when shiftLeft =>
                    PC_inc <= '1';
                    PTR_dec <= '0';
                    mux_sel <= data;
                    FSM_state <= start;

                when inc =>
                    DATA_RDWR <= '1';
                    FSM_state <= inc_wait;

                when inc_wait =>
                    PC_inc <= '1';
                    DATA_WDATA <= DATA_RDATA + 1;
                    FSM_state <= start;

                when dec =>
                    DATA_RDWR <= '1';
                    FSM_state <= dec_wait;

                when dec_wait =>
                    PC_inc <= '1';
                    DATA_WDATA <= DATA_RDATA - 1;
                    FSM_state <= start;

                when whileBegin =>
                    FSM_state <= whileWaitBegin;

                when whileWaitBegin =>
                    PC_inc <= '1';
                    if DATA_RDATA = x"00" then
                        FSM_state <= whileSkip;
                    else
                        FSM_state <= start;
                    end if;

                when whileSkipWait =>
                    PC_inc <= '0';
                    if DATA_RDATA = x"5B" then
                        CNT_reg <= CNT_reg + 1;
                    elsif DATA_RDATA = x"5D" then
                        CNT_reg <= CNT_reg - 1;
                    end if;

                    FSM_state <= whileSkip;

                when whileSkip =>
                    mux_sel <= program;
                    PC_inc <= '1';
                    if CNT_reg = x"00" then
                        PC_inc <= '0';
                        FSM_state <= start;
                    else
                        FSM_state <= whileSkipWait;
                    end if;

                when whileEnd =>
                    FSM_state <= whileWaitEnd;

                when whileWaitEnd =>
                    if DATA_RDATA = x"00" then
                        PC_inc <= '1';
                        FSM_state <= start;
                    else
                        FSM_state <= whileGoBackWait;
                        PC_dec <= '1';
                    end if;

                when whileGoBackWait =>
                    PC_dec <= '0';
                    mux_sel <= program;
                    FSM_state <= whileGoBack;
                    if CNT_reg = x"00" then
                        PC_inc <= '1';
                        FSM_state <= start;
                    end if;

                when whileGoBack =>
                    FSM_state <= whileGoBackDelay;

                when whileGoBackDelay =>
                    PC_dec <= '1';
                    if DATA_RDATA = x"5D" then
                        CNT_reg <= CNT_reg + 1;
                    elsif DATA_RDATA = x"5B" then
                        CNT_reg <= CNT_reg - 1;
                    end if;
                    FSM_state <= whileGoBackWait;

                when doWhileBegin =>
                    PC_inc <= '1';
                    FSM_state <= start;

                when doWhileEnd =>
                    FSM_state <= doWhileWaitEnd;

                when doWhileWaitEnd =>
                    if DATA_RDATA = x"00" then
                        PC_inc <= '1';
                        FSM_state <= start;
                    else 
                        FSM_state <= doWhileGoBackWait;
                        PC_dec <= '1';
                    end if;

                when doWhileGoBackWait =>
                    PC_dec <= '0';
                    mux_sel <= program;
                    FSM_state <= doWhileGoBack;
                    if CNT_reg = x"00" then
                        PC_inc <= '1';
                        FSM_state <= start;
                    end if;

                when doWhileGoBack =>
                    FSM_State <= doWhileGoBackDelay;

                when doWhileGoBackDelay =>
                    PC_dec <= '1';
                    if DATA_RDATA = x"29" then
                        CNT_reg <= CNT_reg + 1;
                    elsif DATA_RDATA = x"28" then
                        CNT_reg <= CNT_reg - 1;
                    end if;
                    FSM_state <= doWhileGoBackWait;


                when output =>
                    if OUT_BUSY = '1' then
                        FSM_State <= output_wait;
                    elsif OUT_BUSY = '0' then
                        OUT_DATA <= DATA_RDATA;
                        OUT_WE <= '1';
                        PC_inc <= '1';
                        DATA_EN <= '0';
                        FSM_State <= start;
                    end if;

                when output_wait =>
                    if OUT_BUSY = '0' then
                        FSM_State <= output;
                    end if;

                when input =>
                    DATA_WDATA <= IN_DATA;
                    PC_inc <= '1';
                    FSM_state <= start;

                when input_wait =>
                    if IN_VLD = '1' then
                        IN_REQ <= '0';
                        DATA_RDWR <= '1';
                        DATA_EN <= '1';
                        FSM_State <= input;
                    end if;

                
                when others =>

            end case;
        end if;
    end process;
end behavioral;

