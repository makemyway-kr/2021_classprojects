package sp21_simulator;

import java.io.IOException;
import java.util.ArrayList;

// instruction에 따라 동작을 수행하는 메소드를 정의하는 클래스

public class InstLuncher {
    ResourceManager rMgr;
    public InstLuncher(ResourceManager resourceManager) {
        this.rMgr = resourceManager;
    }

    public void add(int curr,char[]instruction)
    {
    	Instruction temi=new Instruction(instruction);
    	int []flags=new int[6];
    	flags=temi.getflag();
    	int add=temi.addr(curr,flags);
    	int temp=rMgr.getRegister(0);
    	rMgr.setRegister(0, temp+add);
    }
    public void ADDR(int curr,char[]instruction)
    {
    	Instruction temi=new Instruction(instruction);
    	int []regs=temi.registers();
    	int r1=rMgr.getRegister(regs[0]);
    	int r2=rMgr.getRegister(regs[1]);
    	rMgr.setRegister(r2, r2+r1);
    	rMgr.setRegister(8, curr+temi.format());
    }
    public void CLEAR(int curr,char[]instruction)
    {
    	Instruction temi=new Instruction(instruction);
    	int []regs=temi.registers();
    	rMgr.setRegister(regs[0], 0xFF);
    	rMgr.setRegister(8, curr+temi.format());
    }
    public void COMP(int curr,char[]instruction)
    {
    	Instruction temi=new Instruction(instruction);
    	int []flags=new int[6];
    	flags=temi.getflag();
    	int add=temi.addr(curr,flags);
    	if(add==rMgr.getRegister(0))
    	{
    		rMgr.setRegister(9, 0);
    	}
    	else if(rMgr.getRegister(0)<add)
    	{
    		rMgr.setRegister(9, -1);
    	}
    	else
    	{
    		rMgr.setRegister(9, 1);
    	}
    	rMgr.setRegister(8, curr+temi.format());
    }
    public void COMPR(int curr,char[]instruction)
    {
    	Instruction temi=new Instruction(instruction);
    	int []regs=temi.registers();
    	if(rMgr.getRegister(regs[0])==rMgr.getRegister(regs[1]))
    	{
    		rMgr.setRegister(9,0);
    	}
    	else if(rMgr.register[regs[0]]<rMgr.register[regs[1]])
    	{
    		rMgr.setRegister(9, -1);
    	}
    	else
    	{
    		rMgr.setRegister(9, 1);
    	}
    	rMgr.setRegister(8, curr+temi.format());
    }
    public void DIV(int curr,char[]instruction)
    {
    	Instruction temi=new Instruction(instruction);
    	int []flags=new int[6];
    	flags=temi.getflag();
    	int add=temi.addr(curr,flags);
    	rMgr.setRegister(0,rMgr.getRegister(0)/add);
    	rMgr.setRegister(8, curr+temi.format());
    }
    public void DIVR(int curr,char[]instruction)
    {
    	Instruction temi=new Instruction(instruction);
    	int []regs=temi.registers();
    	rMgr.setRegister(regs[1], rMgr.getRegister(regs[1])/rMgr.getRegister(regs[0]));
    	rMgr.setRegister(8, curr+temi.format());
    }
    public void J(int curr,char[]instruction)
    {
    	Instruction temi=new Instruction(instruction);
    	int []flags=new int[6];
    	flags=temi.getflag();
    	int add=temi.addr(curr,flags);
    	rMgr.setRegister(8, add);
    	//pc register에 대상 address저장
    }
    public void JEQ(int currloc,char[]instruction)
    {
    	Instruction temi=new Instruction(instruction);
    	int []flags=new int[6];
    	flags=temi.getflag();
    	int add=temi.addr(currloc,flags);
    	if(rMgr.getRegister(9)==0)
    	{
    		rMgr.setRegister(8,add);
    	}
    	else
    	{
    		rMgr.setRegister(8, currloc+temi.format());
    	}
    }
    public void JLT(int currloc,char[]instruction)
    {
    	Instruction temi=new Instruction(instruction);
    	int []flags=new int[6];
    	flags=temi.getflag();
    	int add=temi.addr(currloc,flags);
    	if(rMgr.getRegister(9)<0)
    	{
    		rMgr.setRegister(8,add);
    	}
    	else
    	{
    		rMgr.setRegister(8,currloc+temi.format());
    	}
    }
    public void JSUB(int curr,char[]instruction)
    {
    	Instruction temi=new Instruction(instruction);
    	int []flags=new int[6];
    	flags=temi.getflag();
    	int add=temi.addr(curr,flags);
    	rMgr.setRegister(2, rMgr.getRegister(8));
    	rMgr.setRegister(8, add);
    }
    public void LDA(int curr,char[]instruction) 
    {
    	Instruction temi=new Instruction(instruction);
    	int []flags=new int[6];
    	flags=temi.getflag();
    	int add=temi.addr(curr,flags);
    	rMgr.setRegister(0, add);
    	rMgr.setRegister(8, curr+temi.format());
    }
    public void LDCH(int curr,char[]instruction)
    {
    	Instruction temi=new Instruction(instruction);
    	int []flags=new int[6];
    	flags=temi.getflag();
    	int add=temi.addr(curr,flags);
    	int target=add+(rMgr.getRegister(1)/2);
    	char [] temp=rMgr.getMemory(target, 1);
    	rMgr.setRegister(0,temp);
    	rMgr.setRegister(8, curr+temi.format());
    }
    public void RD(int curr,char[]instruction) throws IOException
    {
    	Instruction temi=new Instruction(instruction);
    	int []flags=new int[6];
    	flags=temi.getflag();
    	int add=temi.addr(curr,flags);
    	String devname=String.valueOf(rMgr.getMemory(add,2));
    	rMgr.setRegister(0,rMgr.readDevice(devname, 1));
    	rMgr.setRegister(8, curr+temi.format());
    }
    public void RSUB(int curr,char[]instruction)
    {
    	rMgr.setRegister(8, rMgr.getRegister(2));
    }
    public void STA(int curr,char[]instruction)
    {
    	Instruction temi=new Instruction(instruction);
    	int []flags=new int[6];
    	flags=temi.getflag();
    	int add=temi.addr(curr,flags);
    	rMgr.setMemory(add,Integer.toHexString(rMgr.getRegister(0)).toCharArray(),1);
    	rMgr.setRegister(8, curr+temi.format());
    }
    public void STB(int curr,char[]instruction)
    {
    	Instruction temi=new Instruction(instruction);
    	int []flags=new int[6];
    	flags=temi.getflag();
    	int add=temi.addr(curr,flags);
    	rMgr.setMemory(add,Integer.toHexString(rMgr.getRegister(3)).toCharArray(),1);
    	rMgr.setRegister(8, curr+temi.format());
    }
    public void STCH(int curr,char[]instruction)
    {
    	Instruction temi=new Instruction(instruction);
    	int []flags=new int[6];
    	flags=temi.getflag();
    	int add=temi.addr(curr,flags);
    	char[]temp=new char[1];
    	temp[0]=(char)rMgr.getRegister(0);
    	rMgr.setMemory(add, temp, 1);
    	rMgr.setRegister(8, curr+temi.format());
    }
    public void STL(int curr,char[]instruction)
    {
    	Instruction temi=new Instruction(instruction);
    	int []flags=new int[6];
    	flags=temi.getflag();
    	int add=temi.addr(curr,flags);
    	char[]temp=new char[1];
    	temp[0]=(char)rMgr.getRegister(2);
    	rMgr.setMemory(add, temp, 1);
    	rMgr.setRegister(8, curr+temi.format());
    }
    public void STX(int curr,char[]instruction)
    {
    	Instruction temi=new Instruction(instruction);
    	int []flags=new int[6];
    	flags=temi.getflag();
    	int add=temi.addr(curr,flags);
    	char[]temp=new char[1];
    	temp[0]=(char)rMgr.getRegister(1);
    	rMgr.setMemory(add, temp, 1);
    	rMgr.setRegister(8, curr+temi.format());
    }
    public void TD(int curr,char[]instruction)
    {
    	
    }
    public void TIX(int curr,char[]instruction)
    {
    	
    }
    public void TIXR(char[]instruction)
    {
    	
    }
    public void WD(int curr,char[]instruction)
    {
    	
    }
        
    
    }
class Instruction{
	char[]instruction;
	Instruction(char[]instruction){
		this.instruction=instruction.clone();
	}
	int[] getflag(){
		int []flags=new int[6];
		return flags;
	}
	int addr(int curr,int[] flags) {
		return 0;
	}
	int [] registers() {
		int []temp=new int[2];
		return temp;
	}
	int format()
	{
		int format=0;
		return format;
	}
}