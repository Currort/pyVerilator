`include  "../include/QT.v"
`timescale 1ns / 1ps
/* verilator lint_off UNUSEDSIGNAL */
module SSS64(  
	output   reg     [31:0] data_o           ,
	input wire  [63:0]      data_addr_i      ,
	input wire 	  		    finish_i           
    );

    reg  [7:0] mem_r [(2**10)-1:0];            
    initial begin
        $readmemh(`PATH, mem_r);
    end
    //? 小端模式访存
    always @(*) begin
        data_o[7:0]    = mem_r [data_addr_i[9:0]];
        data_o[15:8]   = mem_r [data_addr_i[9:0]+1];
        data_o[23:16]  = mem_r [data_addr_i[9:0]+2];
        data_o[31:24]  = mem_r [data_addr_i[9:0]+3];
    end
/* verilator lint_on UNUSEDSIGNAL */
endmodule
