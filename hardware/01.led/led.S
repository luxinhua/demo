.text 
.globl _start
_start:
     
    /*
     * set GPM4_0 as output
     */
    ldr r0, =0x110002E0  // GPM4CON�ĵ�ַ��0x110002E0
    ldr r1, [r0]         // �ȶ���ԭֵ
    bic r1, r1, #0xf     // ���bit[3:0]
    orr r1, r1, #1       // ����bit[3:0]Ϊ0b0001
    str r1, [r0]         // д��GPM4CON


    /*
     *  set GPM4_0 as High
     */
    ldr r0, =0x110002E4  // GPM4DAT�ĵ�ַ��0x110002E4
    ldr r1, [r0]         // ����ԭֵ
    bic r1, r1, #1       // ���bit[0]Ϊ0
    str r1, [r0]         // д��GPM4_0

halt_loop:
    b halt_loop
    
    


    
    