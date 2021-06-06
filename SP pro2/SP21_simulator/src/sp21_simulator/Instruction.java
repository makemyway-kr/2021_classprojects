package sp21_simulator;

class Instruction{
	char[]instruction;
	int format=0;
	Instruction(char[]instruction){
		this.instruction=instruction.clone();
	}
	int[] getflag(){
		int []flags=new int[6];
		format=3;
		for(int i=0;i<6;i++)
		{
			flags[i]=0;
		}
		if((this.instruction[1]&2)==2)//n
		{
			flags[0]=1;
		}
		if((this.instruction[1]&1)==1)//i
		{
			flags[1]=1;
		}
		if((this.instruction[2]&8)==8)//x
		{
			flags[2]=1;
		}
		if((this.instruction[2]&4)==4)//b
		{
			flags[3]=1;
		}
		if((this.instruction[2]&2)==2)//p
		{
			flags[4]=1;
		}
		if((this.instruction[2]&1)==1)//e
		{
			flags[5]=1;
			format=4;
		}
		return flags;
	}
	int addr(int curr,int[] flags) {
		int returningadd=0;
		if(flags[4]==1)//pc relative
		{
			curr+=format;
			String temp=this.instruction.toString();
			returningadd=Integer.parseInt(temp.substring(3),16);
			returningadd+=curr;
		}
		else//ref
		{
			String temp=this.instruction.toString();
			returningadd=Integer.parseInt(temp.substring(3),16);
		}
		return returningadd;
	}
	int [] registers() {
		int []temp=new int[2];
		temp[0]=Character.getNumericValue(instruction[2]);
		temp[1]=Character.getNumericValue(instruction[3]);
		format=2;
		return temp;
	}
	int format()
	{
		return format;
	}
}