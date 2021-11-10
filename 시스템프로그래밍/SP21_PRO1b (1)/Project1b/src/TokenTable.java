import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.io.*;
/**
 * 사용자가 작성한 프로그램 코드를 단어별로 분할 한 후, 의미를 분석하고, 최종 코드로 변환하는 과정을 총괄하는 클래스이다.
 * 
 * pass2에서 object code로 변환하는 과정은 혼자 해결할 수 없고 symbolTable과 instTable의 정보가 필요하므로
 * 이를 링크시킨다. section 마다 인스턴스가 하나씩 할당된다.
 *
 */
public class TokenTable {
	public static final int MAX_OPERAND = 3;

	/* bit 조작의 가독성을 위한 선언 */
	public static final int nFlag = 32;
	public static final int iFlag = 16;
	public static final int xFlag = 8;
	public static final int bFlag = 4;
	public static final int pFlag = 2;
	public static final int eFlag = 1;

	/* Token을 다룰 때 필요한 테이블들을 링크시킨다. */
	LabelTable symTab;
	LabelTable literalTab;
	InstTable instTab;

	/** 각 line을 의미별로 분할하고 분석하는 공간. */
	ArrayList<Token> tokenList;
	ArrayList<String>reference;//reference를 별도로 저장하고자함.
	ArrayList<String>EXTDEF;
	/**
	 * 초기화하면서 symTable과 instTable을 링크시킨다.
	 * 
	 * @param symTab    : 해당 section과 연결되어있는 symbol table
	 * @param literaTab : 해당 section과 연결되어있는 literal table
	 * @param instTab   : instruction 명세가 정의된 instTable
	 */
	public TokenTable(LabelTable symTab, LabelTable literalTab, InstTable instTab) {
		this.symTab=symTab;
		this.literalTab=literalTab;
		this.instTab=instTab;
		tokenList=new ArrayList<Token>();
		reference=new ArrayList<String>();
		EXTDEF=new ArrayList<String>();
	}

	/**
	 * 일반 문자열을 받아서 Token단위로 분리시켜 tokenList에 추가한다.
	 * 
	 * @param line : 분리되지 않은 일반 문자열
	 */
	public void putToken(String line) {
		tokenList.add(new Token(line));
	}

	/**
	 * tokenList에서 index에 해당하는 Token을 리턴한다.
	 * 
	 * @param index
	 * @return : index번호에 해당하는 코드를 분석한 Token 클래스
	 */
	public Token getToken(int index) {
		return tokenList.get(index);
	}

	/**
	 * Pass2 과정에서 사용한다. instruction table, symbol table 등을 참조하여 objectcode를 생성하고, 이를
	 * 저장한다.
	 * 
	 * @param index
	 */
	public void makeObjectCode(int index) {
		HashMap<String,String>registers=new HashMap<String,String>();
		registers.put("A", "0");
		registers.put("X", "1");
		registers.put("S", "4");
		registers.put("T", "5");
		registers.put("", "0");
		
		
		Token temperary=getToken(index);
		if(temperary.operator.indexOf("+")!=-1) {
			temperary.setFlag(TokenTable.eFlag, 1);
		}
		else
		{
			temperary.setFlag(TokenTable.eFlag, 0);
		}
		if(temperary.operand[0].indexOf("@")!=-1)
		{
			temperary.setFlag(TokenTable.nFlag, 1);
		}
		else
		{
			temperary.setFlag(TokenTable.nFlag, 0);
		}
		if(temperary.operand[0].indexOf("#")!=-1)
		{
			temperary.setFlag(TokenTable.iFlag, 1);
			
		}
		else
		{
			temperary.setFlag(TokenTable.iFlag, 0);
		}
		if(temperary.operand[1].equals("X"))
		{
			temperary.setFlag(TokenTable.xFlag, 1);
		}
		else
		{
			temperary.setFlag(TokenTable.xFlag, 0);
		}
		if(temperary.getFlag(TokenTable.nFlag)==0 && temperary.getFlag(TokenTable.iFlag)==0)
		{
			temperary.setFlag(TokenTable.nFlag, 1);
			temperary.setFlag(TokenTable.iFlag, 1);//simple addressing
		}
		int is_reference_=0;
		for(int re=0;re<reference.size();re++)
		{
			if(temperary.operand[0].contains(reference.get(re))==true)//reference를 operand로 가지는 경우 p register 0
			{
				is_reference_=1;
			}
		}
		if(is_reference_==0)
		{
			temperary.setFlag(TokenTable.pFlag, 1);
		}
		//objectcode
		if(temperary.getFlag(TokenTable.nFlag)==1 && temperary.getFlag(TokenTable.iFlag)==1 )
		{
			temperary.objectCode+=(Integer.toHexString(instTab.instMap.get(temperary.operator).opcode+3));
			
		}
		else if(temperary.getFlag(TokenTable.nFlag)==1)
		{
			temperary.objectCode+=(Integer.toHexString(instTab.instMap.get(temperary.operator).opcode+2));
		}
		else if(temperary.getFlag(TokenTable.iFlag)==1) 
		{
			temperary.objectCode+=(Integer.toHexString(instTab.instMap.get(temperary.operator).opcode+1));
		}
		int xbpe=0;
		if(temperary.getFlag(TokenTable.xFlag)==1)
		{
			xbpe+=8;
		}
		if(temperary.getFlag(TokenTable.pFlag)==1)
		{
			xbpe+=2;
		}
		if(temperary.getFlag(TokenTable.eFlag)==1)
		{
			xbpe+=1;
		}
		temperary.objectCode+=Integer.toString(xbpe);
		if(instTab.instMap.get(temperary.operator)!=null)
		{
			if(instTab.instMap.get(temperary.operator).format==1)
			{
				temperary.objectCode=Integer.toHexString(instTab.instMap.get(temperary.operator).opcode);
			}
			else if(instTab.instMap.get(temperary.operator).format==2)
			{
				temperary.objectCode=Integer.toHexString(instTab.instMap.get(temperary.operator).opcode)+registers.get(temperary.operand[0])+registers.get(temperary.operand[1]);
			}
			else if(instTab.instMap.get(temperary.operator).format==3)
			{
				if(temperary.operand[0].contains("="))
				{
					if(index<tokenList.size()-1)
					{
						Token cal=getToken(index+1);
						String[] ttttl=temperary.operand[0].split("\'");
						int temlit=literalTab.search(ttttl[1]);
						int tempob=temlit-cal.location;
						temperary.objectCode+=Integer.toHexString(tempob);
					}
					
				}
				else if(temperary.operand[0].contains("#"))
				{
					temperary.objectCode+=temperary.operand[0].substring(1);
				}
				else if(temperary.operand[0].contains("@"))
				{
					if(index<tokenList.size()-1)
					{
						Token cal=getToken(index+1);
						int temlit=symTab.search(temperary.operand[0].substring(1));
						int tempob=temlit-cal.location;
						if(Integer.toHexString(tempob).length()<3)
						{
							if(Integer.toHexString(tempob).length()==1)
							{
								temperary.objectCode+=("00"+Integer.toHexString(tempob));
							}
							else if(Integer.toHexString(tempob).length()==2)
							{
								temperary.objectCode+=("0"+Integer.toHexString(tempob));
							}
							else if(Integer.toHexString(tempob).length()==0)
							{
								temperary.objectCode+="000";
							}
						}
						else
						{
							temperary.objectCode+=Integer.toHexString(tempob);
						}
						
					}
					
				}
				else
				{
					int temlit=symTab.search(temperary.operand[0]);
					if(temlit!=-1)
					{
						if(index<tokenList.size()-1)
						{
							Token cal=getToken(index+1);
							if(temlit<cal.location)
							{
								int tem=cal.location-temlit-1;
								tem=4095-tem;
								temperary.objectCode+=Integer.toHexString(tem);
							}
							else
							{
								temperary.objectCode+=Integer.toHexString(temlit-cal.location);
							}
						}
					}
					else
					{
						temperary.objectCode+="000";
					}
					
				}
			
					
					
			}
			else
			{
				temperary.objectCode="";
			}
			
		}
		else if(temperary.operator.contains("+"))//4형식.
		{
			String temoper=temperary.operator.substring(1);
			if(temperary.operand[0].contains("="))
			{
				if(index<tokenList.size()-1)
				{
					Token cal=getToken(index+1);
					String[] ttttl=temperary.operand[0].split("\'");
					int temlit=literalTab.search(ttttl[1]);
					int tempob=temlit-cal.location;
					temperary.objectCode+=Integer.toHexString(tempob);
				}
				
			}
			else if(temperary.operand[0].contains("#"))
			{
				temperary.objectCode+=temperary.operand[0].substring(1);
			}
			else if(temperary.operand[0].contains("@"))
			{
				if(index<tokenList.size()-1)
				{
					Token cal=getToken(index+1);
					int temlit=symTab.search(temperary.operand[0].substring(1));
					int tempob=temlit-cal.location;
					if(Integer.toHexString(tempob).length()<5)
					{
						if(Integer.toHexString(tempob).length()==1)
						{
							temperary.objectCode+=("0000"+Integer.toHexString(tempob));
						}
						else if(Integer.toHexString(tempob).length()==2)
						{
							temperary.objectCode+=("000"+Integer.toHexString(tempob));
						}
						else if(Integer.toHexString(tempob).length()==0)
						{
							temperary.objectCode+="00000";
						}
						else if(Integer.toHexString(tempob).length()==3)
						{
							temperary.objectCode+=("00"+Integer.toHexString(tempob));
						}
						else if(Integer.toHexString(tempob).length()==4)
						{
							temperary.objectCode+=("0"+Integer.toHexString(tempob));
						}
					}
					else
					{
						temperary.objectCode+=Integer.toHexString(tempob);
					}
					
				}
				
			}
			else
			{
				int temlit=symTab.search(temperary.operand[0]);
				if(temlit!=-1)
				{
					if(index<tokenList.size()-1)
					{
						Token cal=getToken(index+1);
						if(temlit<cal.location)
						{
							int tem=cal.location-temlit-1;
							tem=4095-tem;
							temperary.objectCode+=Integer.toHexString(tem);
						}
						else
						{
							temperary.objectCode+=Integer.toHexString(temlit-cal.location);
						}
					}
				}
				else
				{
					temperary.objectCode+="00000";
				}
				
			}
			
			
		}
		else
		{
			temperary.objectCode="";
		}
		tokenList.set(index, temperary);
	
	}

	/**
	 * index번호에 해당하는 object code를 리턴한다.
	 * 
	 * @param index
	 * @return : object code
	 */
	public String getObjectCode(int index) {
		return tokenList.get(index).objectCode;
	}

}

/**
 * 각 라인별로 저장된 코드를 단어 단위로 분할한 후 의미를 해석하는 데에 사용되는 변수와 연산을 정의한다. 의미 해석이 끝나면 pass2에서
 * object code로 변형되었을 때의 바이트 코드 역시 저장한다.
 */
class Token {
	// 의미 분석 단계에서 사용되는 변수들
	int location;
	String label;
	String operator;
	String[] operand;
	String comment;
	char nixbpe;

	// object code 생성 단계에서 사용되는 변수들
	String objectCode;
	int byteSize;

	/**
	 * 클래스를 초기화 하면서 바로 line의 의미 분석을 수행한다.
	 * 
	 * @param line 문장단위로 저장된 프로그램 코드
	 */
	public Token(String line) {
		label="";
		operator="";
		operand=new String[3];
		for(int i=0;i<3;i++) {
			operand[i]="";
		}
		comment="";
		nixbpe=0;
		objectCode="";
		parsing(line);
	}

	/**
	 * line의 실질적인 분석을 수행하는 함수. Token의 각 변수에 분석한 결과를 저장한다.
	 * 
	 * @param line 문장단위로 저장된 프로그램 코드.
	 */
	public void parsing(String line) {
		 String[]splitstring=line.split("\t");
		 ArrayList<String>ss=new ArrayList<>(Arrays.asList(splitstring));
		 char []tmp=line.toCharArray();
		 if(line.indexOf("\t")==0) {//no label
			 operator=splitstring[1];
			 ss.remove(0);
			 ss.remove(0);
		 }
		 else if(tmp[0]=='.') {//only comment
			 comment=line;
			 ss.remove(0);
			 ss.clear();
		 }
		 else {
			 label=splitstring[0];
			 ss.remove(0);
			 operator=splitstring[1];
			 ss.remove(0);
		 }
		 if(ss.size()>0) {//operand또는 comment가 남아있으면
			 if(ss.size()==2) {
				 String temp=ss.remove(0);
				 String[]splitoperands=temp.split(",");
				 for(int i=0;i<splitoperands.length;i++) {
					 operand[i]=splitoperands[i];
				 }
				 comment=ss.remove(0);
			 }
			 else if(ss.size()==1) {//comment또는 operand 둘중 하나만 남아있을때
				 String temp=ss.remove(0);
				 if(temp.indexOf(" ")!=-1) {//띄어쓰기 있으면 코멘트
					 comment=temp;
				 }
				 else {
					 String[]splitoperands=temp.split(",");
					 for(int i=0;i<splitoperands.length;i++) {
						 operand[i]=splitoperands[i];
					 }
				 }
				 }
			 }
		
		 }
	

	/**
	 * n,i,x,b,p,e flag를 설정한다.
	 * 
	 * 
	 * 사용 예 : setFlag(nFlag, 1) 또는 setFlag(TokenTable.nFlag, 1)
	 * 
	 * @param flag  : 원하는 비트 위치
	 * @param value : 집어넣고자 하는 값. 1또는 0으로 선언한다.
	 */
	public void setFlag(int flag, int value) {
		nixbpe+=(flag*value);
	}

	/**
	 * 원하는 flag들의 값을 얻어올 수 있다. flag의 조합을 통해 동시에 여러개의 플래그를 얻는 것 역시 가능하다.
	 * 
	 * 사용 예 : getFlag(nFlag) 또는 getFlag(nFlag|iFlag)
	 * 
	 * @param flags : 값을 확인하고자 하는 비트 위치
	 * @return : 비트위치에 들어가 있는 값. 플래그별로 각각 32, 16, 8, 4, 2, 1의 값을 리턴할 것임.
	 */

	public int getFlag(int flags) {
		return nixbpe & flags;
	}
}
