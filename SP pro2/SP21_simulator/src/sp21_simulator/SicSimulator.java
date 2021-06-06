package sp21_simulator;

import java.io.File;
import java.io.IOException;

/**
 * 시뮬레이터로서의 작업을 담당한다. VisualSimulator에서 사용자의 요청을 받으면 이에 따라 ResourceManager에 접근하여
 * 작업을 수행한다.
 * 
 * 작성중의 유의사항 : 1) 새로운 클래스, 새로운 변수, 새로운 함수 선언은 얼마든지 허용됨. 단, 기존의 변수와 함수들을 삭제하거나
 * 완전히 대체하는 것은 지양할 것. 2) 필요에 따라 예외처리, 인터페이스 또는 상속 사용 또한 허용됨. 3) 모든 void 타입의 리턴값은
 * 유저의 필요에 따라 다른 리턴 타입으로 변경 가능. 4) 파일, 또는 콘솔창에 한글을 출력시키지 말 것. (채점상의 이유. 주석에 포함된
 * 한글은 상관 없음)
 * 
 * 
 * 
 * + 제공하는 프로그램 구조의 개선방법을 제안하고 싶은 분들은 보고서의 결론 뒷부분에 첨부 바랍니다. 내용에 따라 가산점이 있을 수
 * 있습니다.
 */
public class SicSimulator {
	ResourceManager rMgr;
	SicLoader load;
	InstLuncher lunch;
	int currloc;
	public SicSimulator(ResourceManager resourceManager) {
		// 필요하다면 초기화 과정 추가
		this.rMgr = resourceManager;
		this.lunch=new InstLuncher(resourceManager);
	}

	/**
	 * 레지스터, 메모리 초기화 등 프로그램 load와 관련된 작업 수행. 단, object code의 메모리 적재 및 해석은
	 * SicLoader에서 수행하도록 한다.
	 * @throws IOException 
	 */
	public void load(File program) throws IOException {
		load=new SicLoader(rMgr);
		load.load(program);
		currloc=rMgr.sttadd;
	}

	/**
	 * 1개의 instruction이 수행된 모습을 보인다.
	 * @throws IOException 
	 */
	public void oneStep() throws IOException {
		char[]op=rMgr.getMemory(currloc, 1);
		String opc=String.valueOf(op);
		char []eflag=rMgr.getMemory(currloc+1,1);
		String ef=String.valueOf(eflag);
		int efla=Integer.parseInt(ef.substring(0,1),16);
		boolean e;
		if((efla&1)==1)
		{
			e=true;
		}
		else
		{
			e=false;
		}
		if(opc.equals("1B"))//ADD
		{
			if(e==true)
			{
				lunch.add(currloc, rMgr.getMemory(currloc,4));
			}
			else
			{
				lunch.add(currloc, rMgr.getMemory(currloc,3));
			}
			currloc=rMgr.getRegister(8);//PC레지스터의 주소로 이동.
		}
		else if(opc.equals(90))//ADDR
		{
			lunch.ADDR(currloc, rMgr.getMemory(currloc,2));
			currloc=rMgr.getRegister(8);//PC레지스터의 주소로 이동.
		}
		else if(opc.equals("B4"))//clear
		{
			lunch.CLEAR(currloc, rMgr.getMemory(currloc,2));
			currloc=rMgr.getRegister(8);//PC레지스터의 주소로 이동.
		}
		else if(opc.equals("2B"))//COMP
		{
			if(e==true)
			{
				lunch.COMP(currloc, rMgr.getMemory(currloc,4));
			}
			else
			{
				lunch.COMP(currloc, rMgr.getMemory(currloc,3));
			}
			currloc=rMgr.getRegister(8);//PC레지스터의 주소로 이동.
		}
		else if(opc.equals("A0"))//COMPR
		{
			lunch.COMPR(currloc, rMgr.getMemory(currloc,2));
			currloc=rMgr.getRegister(8);//PC레지스터의 주소로 이동.
		}
		else if(opc.equals("27"))//DIV
		{
			if(e==true)
			{
				lunch.DIV(currloc, rMgr.getMemory(currloc,4));
			}
			else
			{
				lunch.DIV(currloc, rMgr.getMemory(currloc,3));
			}
			currloc=rMgr.getRegister(8);//PC레지스터의 주소로 이동
		}
		else if(opc.equals("9C"))//DIVR
		{
			lunch.DIVR(currloc, rMgr.getMemory(currloc,2));
			currloc=rMgr.getRegister(8);//PC레지스터의 주소로 이동.
		}
		else if(opc.equals("3F"))//J
		{
			if(e==true)
			{
				lunch.J(currloc, rMgr.getMemory(currloc,4));
			}
			else
			{
				lunch.J(currloc, rMgr.getMemory(currloc,3));
			}
			currloc=rMgr.getRegister(8);//PC레지스터의 주소로 이동
		}
		else if(opc.equals("33"))//JEQ
		{
			if(e==true)
			{
				lunch.JEQ(currloc, rMgr.getMemory(currloc,4));
			}
			else
			{
				lunch.JEQ(currloc, rMgr.getMemory(currloc,3));
			}
			currloc=rMgr.getRegister(8);//PC레지스터의 주소로 이동
		}
		else if(opc.equals("3B"))//JLT
		{
			if(e==true)
			{
				lunch.JLT(currloc, rMgr.getMemory(currloc,4));
			}
			else
			{
				lunch.JLT(currloc, rMgr.getMemory(currloc,3));
			}
			currloc=rMgr.getRegister(8);//PC레지스터의 주소로 이동
		}
		else if(opc.equals("4B"))//JSUB
		{
			if(e==true)
			{
				lunch.JSUB(currloc, rMgr.getMemory(currloc,4));
			}
			else
			{
				lunch.JSUB(currloc, rMgr.getMemory(currloc,3));
			}
			currloc=rMgr.getRegister(8);//PC레지스터의 주소로 이동
		}
		else if(opc.equals("03")||opc.equals("01"))//LDA
		{
			if(e==true)
			{
				lunch.LDA(currloc, rMgr.getMemory(currloc,4));
			}
			else
			{
				lunch.LDA(currloc, rMgr.getMemory(currloc,3));
			}
			currloc=rMgr.getRegister(8);//PC레지스터의 주소로 이동
		}
		else if(opc.equals("53"))//LDCH
		{
			if(e==true)
			{
				lunch.LDCH(currloc, rMgr.getMemory(currloc,4));
			}
			else
			{
				lunch.LDCH(currloc, rMgr.getMemory(currloc,3));
			}
			currloc=rMgr.getRegister(8);//PC레지스터의 주소로 이동
		}
		else if(opc.equals("DB"))//RD
		{
			if(e==true)
			{
				lunch.RD(currloc, rMgr.getMemory(currloc,4));
			}
			else
			{
				lunch.RD(currloc, rMgr.getMemory(currloc,3));
			}
			currloc=rMgr.getRegister(8);//PC레지스터의 주소로 이동
		}
		else if(opc.equals("4F"))//RSUB
		{
			if(e==true)
			{
				lunch.RSUB(currloc, rMgr.getMemory(currloc,4));
			}
			else
			{
				lunch.RSUB(currloc, rMgr.getMemory(currloc,3));
			}
			currloc=rMgr.getRegister(8);//PC레지스터의 주소로 이동
		}
		else if(opc.equals("0F"))//STA
		{
			if(e==true)
			{
				lunch.STA(currloc, rMgr.getMemory(currloc,4));
			}
			else
			{
				lunch.STA(currloc, rMgr.getMemory(currloc,3));
			}
			currloc=rMgr.getRegister(8);//PC레지스터의 주소로 이동
		}
		else if(opc.equals("7B"))//STB
		{
			if(e==true)
			{
				lunch.STB(currloc, rMgr.getMemory(currloc,4));
			}
			else
			{
				lunch.STB(currloc, rMgr.getMemory(currloc,3));
			}
			currloc=rMgr.getRegister(8);//PC레지스터의 주소로 이동
		}
		else if(opc.equals("57"))//STCH
		{
			if(e==true)
			{
				lunch.STCH(currloc, rMgr.getMemory(currloc,4));
			}
			else
			{
				lunch.STCH(currloc, rMgr.getMemory(currloc,3));
			}
			currloc=rMgr.getRegister(8);//PC레지스터의 주소로 이동
		}
		else if(opc.equals("17"))//STL
		{
			if(e==true)
			{
				lunch.STL(currloc, rMgr.getMemory(currloc,4));
			}
			else
			{
				lunch.STL(currloc, rMgr.getMemory(currloc,3));
			}
			currloc=rMgr.getRegister(8);//PC레지스터의 주소로 이동
		}
		else if(opc.equals("13"))//STX
		{
			if(e==true)
			{
				lunch.STX(currloc, rMgr.getMemory(currloc,4));
			}
			else
			{
				lunch.STX(currloc, rMgr.getMemory(currloc,3));
			}
			currloc=rMgr.getRegister(8);//PC레지스터의 주소로 이동
		}
		else if(opc.equals("E3"))//TD
		{
			if(e==true)
			{
				lunch.TD(currloc, rMgr.getMemory(currloc,4));
			}
			else
			{
				lunch.TD(currloc, rMgr.getMemory(currloc,3));
			}
			currloc=rMgr.getRegister(8);//PC레지스터의 주소로 이동
		}
		else if(opc.equals("B8"))//TIXR
		{
			lunch.TIXR(currloc, rMgr.getMemory(currloc,2));
			currloc=rMgr.getRegister(8);//PC레지스터의 주소로 이동.
		}
		else if(opc.equals("DF"))//WD
		{
			if(e==true)
			{
				lunch.WD(currloc, rMgr.getMemory(currloc,4));
			}
			else
			{
				lunch.WD(currloc, rMgr.getMemory(currloc,3));
			}
			currloc=rMgr.getRegister(8);//PC레지스터의 주소로 이동
		}
		
	}

	/**
	 * 남은 모든 instruction이 수행된 모습을 보인다.
	 * @throws IOException 
	 */
	public void allStep() throws IOException {
		while(true)
		{
			if(currloc==rMgr.proglength)
			{
				break;
			}
			else
			{
				oneStep();
			}
		}
	}

	/**
	 * 각 단계를 수행할 때 마다 관련된 기록을 남기도록 한다.
	 */
	public void addLog(String log) {
	}
}
