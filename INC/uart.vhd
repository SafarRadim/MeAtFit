-- uart.vhd: UART controller - receiving part
-- Author(s): RADIM SAFAR - xsafar27 (at) stud.fit.vutbr.cz 
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

-------------------------------------------------
entity UART_RX is
port(	
    -- INPUT --
    CLK         : in std_logic;
	RST         : in std_logic;
	DIN         : in std_logic;


    -- OUTPUT --
    DOUT_VLD    : out std_logic;

	DOUT        : out std_logic_vector(7 downto 0)
);
end UART_RX;  

-------------------------------------------------
architecture behavioral of UART_RX is
    
    signal mid_ena  : std_logic := '0';
    signal cnt_ena  : std_logic := '0';

    -- MSB to LSB
    signal cnt      : std_logic_vector(3 downto 0) := "0000";
    signal mid      : std_logic_vector(2 downto 0) := "000"; 
    signal clk_cnt  : std_logic_vector(3 downto 0) := "0000";

begin

UART : entity work.UART_FSM(behavioral)
    port map(
        CLK     => CLK,
        RST     => RST,
        DIN     => DIN,
        MID     => mid,
        CNT     => cnt,
  
        MID_ENA => mid_ena,
        CNT_ENA => cnt_ena,
        VLD     => DOUT_VLD   
    );

process (CLK) begin
    if rising_edge(CLK) then
        
        -- Reset --
        if RST = '1' then
            DOUT <= "00000000";
        end if;

        -- Wait Start / Stop --
        if mid = "111" then
            -- Stop
            if cnt_ena = '0' and mid_ena = '0' then
                mid <= "000";
            end if;
        else 
            -- Start
            if cnt_ena = '0' and mid_ena = '0' then
                cnt <= "0000";
            end if;
        end if;

        -- Wait Mid --
        if mid_ena = '1' then
            mid <= mid + 1;
        end if;

        -- Read --
        if cnt_ena = '1' then
            
            if clk_cnt = "1111" then
                -- save bit
                case cnt is
                    when "0000" => DOUT(0) <= DIN;
                    when "0001" => DOUT(1) <= DIN;
                    when "0010" => DOUT(2) <= DIN;
                    when "0011" => DOUT(3) <= DIN;
                    when "0100" => DOUT(4) <= DIN;
                    when "0101" => DOUT(5) <= DIN;
                    when "0110" => DOUT(6) <= DIN;
                    when "0111" => DOUT(7) <= DIN;
                    when others => null;
                end case;

                cnt <= cnt + 1;
                  
                -- clk_cnt reset
                clk_cnt <= "0000";
            else
                -- clk_cnt increment
                clk_cnt <= clk_cnt + 1;
            end if;
        end if;
    end if;
end process;

end behavioral;
