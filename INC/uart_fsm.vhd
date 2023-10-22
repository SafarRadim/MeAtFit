-- uart_fsm.vhd: UART controller - finite state machine
-- Author(s): RADIM SAFAR - xsafar27 (at) fit.vutbr.cz
--
library ieee;
use ieee.std_logic_1164.all;

-------------------------------------------------
entity UART_FSM is
port(
    -- INPUT --
    CLK         : in std_logic;
    RST         : in std_logic;	
    DIN         : in std_logic;
    
    MID         : in std_logic_vector(2 downto 0); -- 3b MSB to LSB
    CNT         : in std_logic_vector(3 downto 0); -- 3b MSB to LSB

    -- OUTPUT --
    MID_ENA     : out std_logic;
    CNT_ENA     : out std_logic;
    VLD         : out std_logic        
);
end entity UART_FSM;

-------------------------------------------------
architecture behavioral of UART_FSM is

    -- "enum" for states
    type STATE_TYPE is (
        WAIT_START, 
        WAIT_MID, 
        READ, 
        WAIT_STOP, 
        DOUT_VALID
    );
    
    signal states : STATE_TYPE := WAIT_START;

begin

process (CLK) begin
    if rising_edge(CLK) then
        VLD <= '0';

        -- Reset signal handle
        if RST = '1' then
            states <= WAIT_START;
            MID_ENA <= '0';
            CNT_ENA <= '0';
            VLD <= '0';

        -- state decision switch/case
        else
            case states is
                when WAIT_START =>
                    if DIN = '0' then
                        states <= WAIT_MID;
                        MID_ENA <= '1';
                        CNT_ENA <= '0';
                    end if;
                        

                when WAIT_MID =>
                    if MID = "111" then
                        states <= READ;
                        MID_ENA <= '0';
                        CNT_ENA <= '1';
                    end if;


                when READ =>
                    if CNT = "1000" then
                        states <= WAIT_STOP;
                    end if;

                when WAIT_STOP =>
                    if DIN = '1' then
                        states <= DOUT_VALID;
                        CNT_ENA <= '0';
                    end if;

                when DOUT_VALID =>
                    states <= WAIT_START;
                    VLD <= '1';

                when others => null;

            end case;
        end if;
    end if;
end process;

end behavioral;
