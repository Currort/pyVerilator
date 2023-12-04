`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/11/10 18:38:54
// Design Name: 
// Module Name: test_rom
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module test_rom(
    output   reg	  		write_en_o         ,
	output   reg     [31:0] 	data_o           ,
	input wire  [63:0]     data_addr_i      ,
	input wire 	  		   finish_i           
    );

    reg  [7:0] mem_r [(2**10)-1:0];            
    initial begin
        $readmemh("`AD", mem_r);
        write_en_o = 0;
        #20 write_en_o = 1;
        #20 write_en_o = 0;
    end
    always @(*) begin
        if(finish_i)begin
            mem_r [2^^10-1] <= 8'hFF;
        end
    end
    //? 小端模式访存
    always @(*) begin
        data_o[7:0]    <= mem_r [data_addr_i[9:0]];
        data_o[15:8]   <= mem_r [data_addr_i[9:0]+1];
        data_o[23:16]  <= mem_r [data_addr_i[9:0]+2];
        data_o[31:24]  <= mem_r [data_addr_i[9:0]+3];
    end

endmodule
