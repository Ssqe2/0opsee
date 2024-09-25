`include "core_struct.vh"
module Core (
    input clk,
    input rst,

    Mem_ift.Master imem_ift,
    Mem_ift.Master dmem_ift,

    output cosim_valid,
    output CorePack::CoreInfo cosim_core_info
);
    import CorePack::*;
    
    // fill your code
     /*typedef struct{
        logic [63:0] pc;   
        logic [63:0] inst;    
        logic [63:0] rs1_id;  
        logic [63:0] rs1_data;
        logic [63:0] rs2_id; 
        logic [63:0] rs2_data;
        logic [63:0] alu;
        logic [63:0] mem_addr;
        logic [63:0] mem_we;
        logic [63:0] mem_wdata;
        logic [63:0] mem_rdata;
        logic [63:0] rd_we;
        logic [63:0] rd_id;   
        logic [63:0] rd_data; 
        logic [63:0] br_taken;
        logic [63:0] npc;
    } CoreInfo;*/
    logic [63:0] pc;   
    logic [31:0] inst;    
    logic [4:0] rs1;  
    logic [63:0] read_data_1;
    logic [4:0] rs2; 
    logic [63:0] read_data_2;
    logic [63:0] alu_res;
    logic  we_reg;
    logic [4:0] rd;   
    logic [63:0]wb_val;
    logic npc_sel;
    logic [63:0] next_pc;
    logic [63:0] alu_op1;
    logic [63:0] alu_op2;
    logic [4:0] alu_opcode;
    logic [63:0] write_to_reg;
    logic [63:0] imm_gen_data;
    logic [63:0] data_packaged;
    logic [63:0] data_from_mem;
    logic is_b;
    logic is_j;
    logic br_taken_;
    logic jalr;
    logic [63:0] inst_64;
    logic [1:0] wrong;
    logic [63:0] data_trun;
    //*****************control模块***************************
    logic [2:0] immgen_op;
    cmp_op_enum cmp_op;
    logic [1:0] alu_asel;
    logic [1:0] alu_bsel;
    logic [1:0] wb_sel;
    mem_op_enum mem_op;
    logic re_mem;
    logic we_mem;
    logic [7:0] mask;
    logic [63:0] shifted_data;
    assign shifted_data = data_from_mem >> (alu_res[2:0]*8);
    controller controller_(
        .inst(inst),
        .we_reg(we_reg),
        .we_mem(we_mem),
        .is_b(is_b),
        .is_j(is_j),
        .immgen_op(immgen_op),
        .alu_op(alu_opcode),
        .cmp_op(cmp_op),
        .alu_asel(alu_asel),
        .alu_bsel(alu_bsel),
        .wb_sel(wb_sel),
        .mem_op(mem_op),
        .jalr(jalr),
        .re_mem(re_mem)
    );
     ALU ALU_(.a(alu_op1),.b(alu_op2),.alu_op(alu_opcode),.res(alu_res));
     //clk可否�?致？
    // 信号连接到Mem_ift_imem
    //
    assign imem_ift.r_request_bits.raddr = pc;
    assign imem_ift.r_request_valid = 1;
    assign inst_64 = imem_ift.r_reply_bits.rdata;
    assign wrong = imem_ift.r_reply_bits.rresp;
    assign imem_ift.r_reply_ready = 1;
    assign imem_ift.w_request_bits.waddr = {64{1'b0}};
    assign imem_ift.w_request_bits.wdata = {64{1'b0}};
    assign imem_ift.w_request_bits.wmask = 8'b00000000;
    assign imem_ift.w_request_valid = 1;
    assign wrong = imem_ift.w_reply_bits.bresp;
    assign imem_ift.w_reply_ready = 1;
   //
    assign dmem_ift.r_request_bits.raddr = alu_res;
    assign dmem_ift.r_request_valid =re_mem ;
    assign data_from_mem = dmem_ift.r_reply_bits.rdata;
    assign wrong = dmem_ift.r_reply_bits.rresp;
    assign dmem_ift.r_reply_ready = 1;
    assign dmem_ift.w_request_bits.waddr = alu_res;
    assign dmem_ift.w_request_bits.wdata = data_packaged;
    assign dmem_ift.w_request_bits.wmask = 8'b11111111;
    assign dmem_ift.w_request_valid = we_mem;
    assign wrong = dmem_ift.w_reply_bits.bresp;
    assign dmem_ift.w_reply_ready = 1;
    // 信号连接到Mem_ift_dmem
    assign npc_sel = is_j | (is_b & br_taken_);
    //修改pc
    always @(posedge clk or posedge rst) begin
        if(rst)begin
            next_pc = 0;
        end
        else begin /*此处存疑*/
            if(npc_sel)begin
                next_pc =  alu_res;
                end
            else begin
                next_pc = pc + 4;
            end
        end
    end
     assign pc = next_pc;

    //拉取指令

    always @(*) begin
        if(pc[2]==1)begin
        inst = inst_64[63:32];
     end
        else begin
        inst = inst_64[31:0];
        end
    end

    //指令译码阶段�?
    assign rd = inst[11:7];
    assign rs1 = inst[19:15];
    assign rs2 = inst[24:20];
    RegFile regfile(.clk(clk),.rst(rst),.we(we_reg),.read_addr_1(rs1),.read_addr_2(rs2),.write_addr(rd),.write_data(wb_val),.read_data_1(read_data_1),.read_data_2(read_data_2));
    //立即数处�?
    imm_gen immgen(.inst(inst),.immgen_op(immgen_op),.imm(imm_gen_data));
    //branch compare
    Cmp branch_cmp(.a(read_data_1),.b(read_data_2),.cmp_op(cmp_op),.cmp_res(br_taken_));
    //alu 前的两个多路选择�?
    always @(*) begin
        case (alu_asel)
            ASEL0:begin
                alu_op1 = 0;
            end
            ASEL_REG:begin
                alu_op1 = read_data_1;
            end 
            ASEL_PC:begin
                alu_op1 = pc;
            end
            //asel3是什�?
            default:begin
                alu_op1 = 0;
            end
        endcase
    end
     always @(*) begin
        case (alu_bsel)
            BSEL0:begin
                alu_op2 = 0;
            end
            BSEL_REG:begin
                alu_op2 = read_data_2;
            end 
            BSEL_IMM:begin
                alu_op2 = imm_gen_data;
            end
            //bsel3是什�?
            default: begin
                alu_op2 =0;
            end
        endcase
    end


    //dmem操作
    //*********write
    //data package操作�?
    data_package data_package_(.shift(alu_res[2:0]),.data(read_data_2),.mem_op_enum_(mem_op),.data_packaged_reg_(data_packaged));
    //musk 操作�?
    mask_gen mask_gen(.mem_op_enum(mem_op),.mask(mask));
    //*********read
    data_truction data_truction_(.data(shifted_data),.data_to_reg(data_trun),.mem_op_enum_(mem_op));
    //*********write_reg
    always_comb begin
        case (wb_sel)
            WB_SEL0: begin
                write_to_reg = 0;
            end 
            WB_SEL_ALU:begin
                write_to_reg = alu_res;
            end
            WB_SEL_PC:begin
                write_to_reg = pc + 4;
            end
            WB_SEL_MEM:begin
                write_to_reg = data_trun;
            end
            default:begin
                write_to_reg = 0;
            end
        endcase
    end

    assign wb_val = write_to_reg;
    assign cosim_valid = 1'b1;
    assign cosim_core_info.pc        = pc;
    assign cosim_core_info.inst      = {32'b0,inst};   
    assign cosim_core_info.rs1_id    = {59'b0, rs1};
    assign cosim_core_info.rs1_data  = read_data_1;
    assign cosim_core_info.rs2_id    = {59'b0, rs2};
    assign cosim_core_info.rs2_data  = read_data_2;
    assign cosim_core_info.alu       = alu_res;
    assign cosim_core_info.mem_addr  = dmem_ift.r_request_bits.raddr;
    assign cosim_core_info.mem_we    = {63'b0, dmem_ift.w_request_valid};
    assign cosim_core_info.mem_wdata = dmem_ift.w_request_bits.wdata;
    assign cosim_core_info.mem_rdata = dmem_ift.r_reply_bits.rdata;
    assign cosim_core_info.rd_we     = {63'b0, we_reg};
    assign cosim_core_info.rd_id     = {59'b0, rd}; 
    assign cosim_core_info.rd_data   = wb_val;
    assign cosim_core_info.br_taken  = {63'b0, npc_sel};
    assign cosim_core_info.npc       = next_pc;

endmodule
//****************imm_gen*****************
module imm_gen (
    input [31:0]inst,
    input [2:0]immgen_op,
    output [63:0] imm
);  
    logic [63:0] imm_reg;
    import CorePack::*;
    assign imm = imm_reg;
    always @(*) begin
        case (immgen_op)
            IMM0: begin
                imm_reg = 64'b0; // 默认�?
            end
            I_IMM: begin
                imm_reg = {{52{inst[31]}}, inst[31:20]}; // 符号扩展�?64�?
            end
            S_IMM: begin
                imm_reg = {{52{inst[31]}}, inst[31:25], inst[11:7]}; // 拼接后符号扩�?
            end
            B_IMM: begin
                imm_reg = {{51{inst[31]}}, inst[31], inst[7], inst[30:25], inst[11:8], 1'b0}; // 拼接后符号扩�?
            end
            U_IMM: begin
                imm_reg = {{32{inst[31]}},inst[31:12],12'b0}; // 直接移位到高�?
            end
            UJ_IMM: begin
                imm_reg = {{43{inst[31]}}, inst[31], inst[19:12], inst[20], inst[30:21], 1'b0}; // 拼接后符号扩�?
            end
            IMM6: begin
                // 其他类型立即数处理�?�辑
                imm_reg = 0; // 占位�?
            end
            IMM7: begin
                // 其他类型立即数处理�?�辑
                imm_reg = 0; // 占位�?
            end
            default: begin
                imm_reg = 0; // 默认�?
            end
        endcase
    end
endmodule

//***************data_package****************
module data_package (
    input [2:0] shift,
    input [63:0]data,
    input [2:0]mem_op_enum_,
    output logic[63:0]data_packaged_reg_
);
     import CorePack::*;
     data_t shifted_data;
     data_t data_packaged_reg;
   always_comb begin
    case (mem_op_enum_)
        MEM_NO: begin
            data_packaged_reg = data;
        end
        MEM_D:begin
            data_packaged_reg = data;
        end
        MEM_W:begin
            data_packaged_reg = {data[31:0],data[31:0]};
        end
        MEM_H:begin
            data_packaged_reg = {data[15:0],data[15:0],data[15:0],data[15:0]};
        end
        MEM_B:begin
            data_packaged_reg = {8{data[7:0]}};
        end
        MEM_UB:begin
            data_packaged_reg =  {8{data[7:0]}};
        end
        MEM_UH:begin
            data_packaged_reg = {4{data[15:0]}};
        end
        MEM_UW:begin
            data_packaged_reg = {2{data[31:0]}};
        end
        default: begin
            
        end
    endcase
    shifted_data = data_packaged_reg << (shift*8);
    data_packaged_reg_ = shifted_data;
   end
   
endmodule

//*******************mask模块******************
module mask_gen (
    input [2:0]mem_op_enum,
    output logic[7:0]mask
);
     import CorePack::*;
    always_comb begin
         case (mem_op_enum)
            MEM_NO: begin
                mask = 8'b00000000;
            end
            MEM_D:begin
                mask = 8'b11111111;
            end
            MEM_W:begin
                mask = {4'b0000,4'b1111};
            end
            MEM_H:begin
                mask = {6'b000000,2'b11};
            end
            MEM_B:begin
                mask = {7'b0000000,1'b1};
            end
            MEM_UB:begin    
                mask = {7'b0000000,1'b1};
            end
            MEM_UH:begin
                mask = {6'b000000,2'b11};
            end
            MEM_UW:begin
                mask = {4'b0000,4'b1111};
            end
            default: begin
                mask = 8'b00000000;
                
            end
         endcase
    end
endmodule
//*********data_truction*************

module data_truction (
    input [63:0]data,
    input CorePack::mem_op_enum mem_op_enum_,
    output logic [63:0]data_to_reg
);  
     import CorePack::*;
    always_comb begin 
        case (mem_op_enum_)
            MEM_NO:begin
                data_to_reg = data;
            end
            MEM_D:begin
                data_to_reg = data;
            end
            MEM_W:begin
                data_to_reg = {{32{data[31]}},data[31:0]};
            end
            MEM_H:begin
                data_to_reg = {{48{data[15]}},data[15:0]};
            end
            MEM_B:begin
                data_to_reg = {{56{data[7]}},data[7:0]};
            end
            MEM_UB:begin
                data_to_reg = {{56{1'b0}},data[7:0]};
            end
            MEM_UH:begin
                data_to_reg = {{48{1'b0}},data[15:0]};
            end
            MEM_UW:begin
                data_to_reg = {{32{1'b0}},data[31:0]};
            end
            default: begin
                data_to_reg = 0;
            end
        endcase
    end
endmodule
module MultiFSM(
    input clk,
    input rst,
    Mem_ift.Master imem_ift,
    Mem_ift.Master dmem_ift,
    input we_mem,
    input re_mem,
    input CorePack::addr_t pc,
    input CorePack::addr_t alu_res,
    input CorePack::data_t data_package,
    input CorePack::mask_t mask_package,
    output stall
);
    import CorePack::*;

    // fill your code for bonus


endmodule