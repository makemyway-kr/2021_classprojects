import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.io.*;
/**
 * Assembler: 이 프로그램은 SIC/XE 머신을 위한 Assembler 프로그램의 메인루틴이다. 프로그램의 수행 작업은 다음과 같다.
 * 1) 처음 시작하면 Instruction 명세를 읽어들여서 assembler를 세팅한다.
 * 
 * 2) 사용자가 작성한 input 파일을 읽어들인 후 저장한다
 * 
 * 3) input 파일의 문장들을 단어별로 분할하고 의미를 파악해서 정리한다. (pass1)
 * 
 * 4) 분석된 내용을바탕으로 컴퓨터가 사용할 수 있는 object code를 생성한다. (pass2)
 * 
 * 
 * 작성중의 유의사항:
 * 
 * 1) 새로운 클래스, 새로운 변수, 새로운 함수 선언은 얼마든지 허용됨. 단, 기존의 변수와 함수들을 삭제하거나 완전히 대체하는 것은
 * 안된다.
 * 
 * 2) 마찬가지로 작성된 코드를 삭제하지 않으면 필요에 따라 예외처리, 인터페이스 또는 상속 사용 또한 허용됨
 * 
 * 3) 모든 void 타입의 리턴값은 유저의 필요에 따라 다른 리턴 타입으로 변경 가능.
 * 
 * 4) 파일, 또는 콘솔창에 한글을 출력시키지말 것. (채점상의 이유. 주석에 포함된 한글은 상관 없음)
 * 
 * + 제공하는 프로그램 구조의 개선방법을 제안하고 싶은 분들은 보고서의 결론 뒷부분에 첨부 바랍니다. 내용에 따라 가산점이 있을 수
 * 있습니다.
 */

public class Assembler {
	/** instruction 명세를 저장한 공간 */
	public InstTable instTable;
	/** 읽어들인 input 파일의 내용을 한 줄 씩 저장하는 공간. */
	ArrayList<String> lineList;
	/** 프로그램의 section별로 symbol table을 저장하는 공간 */
	ArrayList<LabelTable> symtabList;
	/** 프로그램의 section별로 literal table을 저장하는 공간 */
	ArrayList<LabelTable> literaltabList;
	/** 프로그램의 section별로 프로그램을 저장하는 공간 */
	ArrayList<TokenTable> TokenList;
	/**
	 * Token, 또는 지시어에 따라 만들어진 오브젝트 코드들을 출력 형태로 저장하는 공간. 필요한 경우 String 대신 별도의 클래스를
	 * 선언하여 ArrayList를 교체해도 무방함.
	 */
	ArrayList<String> codeList;

	/**
	 * 클래스 초기화. instruction Table을 초기화와 동시에 세팅한다.
	 * 
	 * @param instFile : instruction 명세를 작성한 파일 이름.
	 * @throws IOException 
	 */
	public int sttadd;//스타팅주소값
	public Assembler(String instFile) throws IOException {
		instTable = new InstTable(instFile);
		lineList = new ArrayList<String>();
		symtabList = new ArrayList<LabelTable>();
		literaltabList = new ArrayList<LabelTable>();
		TokenList = new ArrayList<TokenTable>();
		codeList = new ArrayList<String>();
	}

	/**
	 * 어셈블러의 메인 루틴
	 * @throws IOException 
	 */
	public static void main(String[] args) throws IOException {
		Assembler assembler = new Assembler("inst.data");
		assembler.loadInputFile("input.txt");
		assembler.pass1();

		assembler.printSymbolTable("symtab_20192698");
		assembler.printLiteralTable("literaltab_20192698");
		assembler.pass2();
		assembler.printObjectCode("output_20192698");

	}

	/**
	 * inputFile을 읽어들여서 lineList에 저장한다.
	 * 
	 * @param inputFile : input 파일 이름.
	 * @throws IOException 
	 */
	private void loadInputFile(String inputFile) throws IOException {
		File input=new File(inputFile);
		FileReader finput=new FileReader(input);
		BufferedReader bufReader=new BufferedReader(finput);
		String line="";
		int i=0;
		while((line=bufReader.readLine())!=null) {
			lineList.add(line);
			line="";
			i++;
		}

	}

	/**
	 * pass1 과정을 수행한다.
	 * 
	 * 1) 프로그램 소스를 스캔하여 토큰 단위로 분리한 뒤 토큰 테이블을 생성.
	 * 
	 * 2) symbol, literal 들을 SymbolTable, LiteralTable에 정리.
	 * 
	 * 주의사항: SymbolTable, LiteralTable, TokenTable은 프로그램의 section별로 하나씩 선언되어야 한다.
	 * 
	 * @param inputFile : input 파일 이름.
	 */
	private void pass1() {
		TokenTable[] tokens=new TokenTable[3];
		LabelTable[] symbols=new LabelTable[3];
		LabelTable[] literals=new LabelTable[3];
		for(int i=0;i<3;i++) {
			symbols[i]=new LabelTable();
			literals[i]=new LabelTable();
			tokens[i]=new TokenTable(symbols[i],literals[i],instTable);
		}
		int locctr=0;
		int section_count=0;
		sttadd=0;
		int curr=0;
		int is_there_literal_to_address=0;
		String literal_to_add=new String();
		int last_locctr=sttadd;
		int movetonext=0;
		for(int i=0;i<lineList.size();i++) 
		{
			tokens[section_count].putToken(lineList.get(i));
			Token temp=tokens[section_count].getToken(curr);
			temp.location=locctr;
			if(i==0) {
				sttadd=Integer.parseInt(temp.operand[0],16);
				temp.location=locctr;
				tokens[section_count].tokenList.set(curr, temp);
			}
			if(temp.operator!="") 
			{
				
				if(temp.operator.equals("CSECT")) 
				{
					last_locctr=locctr;
					locctr=0;//controlsection바뀌어서 초기화해줌.
					temp.location=locctr;
					tokens[section_count+1].tokenList.add(temp);
					tokens[section_count].tokenList.remove(curr);
					movetonext=1;
					curr=0;
					
				}
				else if(temp.operator.equals("EXTREF"))
				{
					for(int refs=0;refs<3;refs++)
					{
						if(!temp.operand[refs].equals(""))
						{
							tokens[section_count].reference.add(temp.operand[refs]);
						}
					}
				}
				else if(temp.operator.equals("EXTDEF"))
				{
					for(int defs=0;defs<3;defs++)
					{
						if(!temp.operand[defs].equals(""))
						{
							tokens[section_count].EXTDEF.add(temp.operand[defs]);
						}
					}
				}
				
			
			}
			if(temp.operator=="" && temp.operand[0]==""&&temp.label==""&&temp.comment!="") 
			{//comment만 존재하는것.
				temp.location=locctr;
				tokens[section_count].tokenList.set(curr, temp);
			}
			if(temp.label!="")
			{
				if(temp.operator.equals("EQU")) 
				{
					Token pretemp=tokens[section_count].getToken(curr-1);
					if(pretemp.operator.equals("EQU"))
					{
						Token temp3=tokens[section_count].getToken(curr-2);
						temp.location=Integer.parseInt(temp3.operand[0],10);
						tokens[section_count].symTab.putName(temp.label,temp.location);
						tokens[section_count].tokenList.set(curr, temp);
					}
					else {
						tokens[section_count].symTab.putName(temp.label, temp.location);
						tokens[section_count].tokenList.set(curr, temp);
					}
			
		        }
				else 
				{
					temp.location=locctr;
					tokens[section_count].symTab.putName(temp.label,temp.location);
					tokens[section_count].tokenList.set(curr, temp);
				}
				
			}
			if(temp.operator!="") {
				Instruction inst=instTable.instMap.get(temp.operator);
				if(temp.operator.indexOf("+")!=-1) {
					locctr+=4;
				}
				
				else if(inst!=null) {
					if(inst.format==2) {
						locctr+=2;
					}
					else if(inst.format==1) {
						locctr+=1;
					}
					else {
						locctr+=3;
					}
				}
				else if(temp.operator.equals("WORD")) {
					locctr+=3;
				}
				else if(temp.operator.equals("RESW")) {
					locctr+=(3*(Integer.parseInt(temp.operand[0],10)));
				}
				else if(temp.operator.equals("RESB")) {
					locctr+=Integer.parseInt(temp.operand[0],10);
					
				}
				else if(temp.operator.equals("LTORG")) {
					locctr+=3;
					if(is_there_literal_to_address>=1)
					{
						tokens[section_count].literalTab.modifyName(literal_to_add, locctr-3);
					}
					literal_to_add="";
					is_there_literal_to_address=0;
				}
				else if(temp.operator.equals("BYTE")) {
					locctr+=1;
				}
			}
			if(temp.operand[0].indexOf("=")==0) {//literal
				String[] temporary;
				temporary=temp.operand[0].split("\'");
				if(tokens[section_count].literalTab.locationList.size()>0)
				{
					if(tokens[section_count].literalTab.search(temporary[1])==-1)
					{
						literal_to_add=temporary[1];
						tokens[section_count].literalTab.putName(temporary[1],555 );//임시값
						is_there_literal_to_address=1;
						
					}
					
				}
				else {
					literal_to_add=temporary[1];
					tokens[section_count].literalTab.putName(temporary[1],555 );//임시값
					is_there_literal_to_address=1;
				}
			}
			if(movetonext==1|| temp.operator.equals("END")) 
			{//지난섹션의 값 저장.
				if(is_there_literal_to_address>=1) {
					if(locctr==0) {
						literals[section_count].modifyName(literal_to_add, last_locctr);
						is_there_literal_to_address-=1;
						literal_to_add="";
					}
					else {
						literals[section_count].modifyName(literal_to_add,locctr);
					}
					
				}
				movetonext=0;
				section_count+=1;
				symtabList.add(tokens[section_count-1].symTab);
				literaltabList.add(tokens[section_count-1].literalTab);
				TokenList.add(tokens[section_count-1]);
	        }
			else {
				tokens[section_count].tokenList.set(curr, temp);
			}
			curr++;//section내에서의 순서.
		
       }//for문끝
	
}

	/**
	 * 작성된 SymbolTable들을 출력형태에 맞게 출력한다.
	 * 
	 * @param fileName : 저장되는 파일 이름
	 * @throws IOException 
	 */
	private void printSymbolTable(String fileName) throws IOException {
		if(fileName=="") {
			for(int i=0;i<3;i++) {
				TokenTable temp=TokenList.get(i);
				LabelTable temperar=temp.symTab;
				for(int j=0;j<temperar.label.size();j++) {
					System.out.print(temperar.label.get(j));
					System.out.print("\t");
					String temmm=Integer.toHexString(temperar.search(temperar.label.get(j)));
					System.out.println(temmm.toUpperCase());
				}
			}
		}
		else {
			File symfile=new File(fileName);
			FileWriter ftowr=new FileWriter(symfile);
			BufferedWriter bw=new BufferedWriter(ftowr);
			for(int i=0;i<3;i++) {
				TokenTable temp=TokenList.get(i);
				LabelTable temperar=temp.symTab;
				for(int j=0;j<temperar.label.size();j++) {
					bw.write(temperar.label.get(j));
					bw.write("\t");
					String temmm=Integer.toHexString(temperar.search(temperar.label.get(j)));
					bw.write(temmm.toUpperCase());
					bw.newLine();
				}
			}
			bw.close();
		}
	}

	/**
	 * 작성된 LiteralTable들을 출력형태에 맞게 출력한다.
	 * 
	 * @param fileName : 저장되는 파일 이름
	 * @throws IOException 
	 */
	private void printLiteralTable(String fileName) throws IOException {
		if(fileName=="") {
			for(int i=0;i<3;i++) {
				TokenTable temp=TokenList.get(i);
				LabelTable temperar=temp.literalTab;
				for(int j=0;j<temperar.label.size();j++) {
					System.out.print(temperar.label.get(j));
					System.out.print("\t");
					String temmm=Integer.toHexString(temperar.search(temperar.label.get(j)));
					System.out.println(temmm.toUpperCase());
				}
			}
		}
		else {
			File litfile=new File(fileName);
			FileWriter ftowr=new FileWriter(litfile);
			BufferedWriter bw=new BufferedWriter(ftowr);
			for(int i=0;i<3;i++) {
				TokenTable temp=TokenList.get(i);
				LabelTable temperar=temp.literalTab;
				for(int j=0;j<temperar.label.size();j++) {
					bw.write(temperar.label.get(j));
					bw.write("\t");
					String temmm=Integer.toHexString(temperar.search(temperar.label.get(j)));
					bw.write(temmm.toUpperCase());
					bw.newLine();
				}
			}
			bw.close();
		}
		

	}

	/**
	 * pass2 과정을 수행한다.
	 * 
	 * 1) 분석된 내용을 바탕으로 object code를 생성하여 codeList에 저장.
	 */
	private void pass2() {
		
		for(int i=0;i<3;i++)//flag 설정
		{
			TokenTable temp=TokenList.get(i);
			for(int j=0;j<temp.tokenList.size();j++)
			{
				temp.makeObjectCode(j);
			}		
				
		}
		

	}

	/**
	 * 작성된 codeList를 출력형태에 맞게 출력한다.
	 * 
	 * @param fileName : 저장되는 파일 이름
	 * @throws IOException 
	 */
	private void printObjectCode(String fileName) throws IOException {
		if(fileName.equals(""))
		{
			for(int i=0;i<3;i++)
			{
				TokenTable temp=TokenList.get(i);
				for(int j=0;j<temp.tokenList.size();j++)
				{
					Token temperary=temp.getToken(j);
					if(!temperary.objectCode.equals(""))
					{
						System.out.println(temperary.objectCode);
					}
				}
					
			}
		}
		else
		{
			File fw=new File(fileName);
			FileWriter fr=new FileWriter(fw);
			BufferedWriter bf=new BufferedWriter(fr);
			for(int i=0;i<3;i++)
			{
				TokenTable temp=TokenList.get(i);
				for(int j=0;j<temp.tokenList.size();j++)
				{
					Token temperary=temp.getToken(j);
					if(!temperary.objectCode.equals(""))
					{
						bf.write(temperary.objectCode);
						
					}
				}
				bf.newLine();
					
			}
			bf.close();
		}

	}

}
