package sp21_simulator;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

/**
 * SicLoader는 프로그램을 해석해서 메모리에 올리는 역할을 수행한다. 이 과정에서 linker의 역할 또한 수행한다.
 * 
 * SicLoader가 수행하는 일을 예를 들면 다음과 같다. - program code를 메모리에 적재시키기 - 주어진 공간만큼 메모리에 빈
 * 공간 할당하기 - 과정에서 발생하는 symbol, 프로그램 시작주소, control section 등 실행을 위한 정보 생성 및 관리
 */
public class SicLoader {
	ResourceManager rMgr;

	public SicLoader(ResourceManager resourceManager) {
		// 필요하다면 초기화
		setResourceManager(resourceManager);
	}

	/**
	 * Loader와 프로그램을 적재할 메모리를 연결시킨다.
	 * 
	 * @param rMgr
	 */
	public void setResourceManager(ResourceManager resourceManager) {
		this.rMgr = resourceManager;
	}

	/**
	 * object code를 읽어서 load과정을 수행한다. load한 데이터는 resourceManager가 관리하는 메모리에 올라가도록
	 * 한다. load과정에서 만들어진 symbol table 등 자료구조 역시 resourceManager에 전달한다.
	 * 
	 * @param objectCode 읽어들인 파일
	 * @throws IOException 
	 */
	int sttadd=0;
	public void load(File objectCode) throws IOException {
		FileReader finput=new FileReader(objectCode);
		BufferedReader bufread=new BufferedReader(finput);
		String line=new String();
		int sttadd=0;//starting address of program
		int length=0;
		int locctr=sttadd;
		while((line=bufread.readLine())!=null) {
			if(line.charAt(0)=='D')
			{
				for(int i=1;i<line.length();i+=12)
				{
					rMgr.symtabList.putSymbol(line.substring(i,i+6), Integer.parseInt(line.substring(i+6,i+12),16));
				}
			}
		}
		bufread.close();
		finput.close();
		FileReader finput2=new FileReader(objectCode);
		BufferedReader bufread2=new BufferedReader(finput2);
		while((line=bufread2.readLine())!=null) {
			if(line.charAt(0)=='H')//Header
			{
				if(locctr==0)
				{
					sttadd=Integer.parseInt(line.substring(7,13),16);
					length=Integer.parseInt(line.substring(13),16);
					locctr=sttadd;
				}
			}
			else if(line.charAt(0)=='M')
			{
				float fixadd=(float)Integer.parseInt(line.substring(1,7),16);
				int numf=Integer.parseInt(line.substring(7,9));
				if(numf%2==1)//홀수일경우 0.5더해주어야함 주소값
				{
					fixadd+=0.5;
				}
				if(line.charAt(9)=='+')
				{
					int temperary=0;
					temperary=Integer.parseInt(String.valueOf(rMgr.getMemory(fixadd, numf)),16);
					String refsym=new String();
					refsym=line.substring(10);
					int toadd=rMgr.symtabList.search(refsym);
					temperary+=toadd;
					rMgr.setMemory(fixadd,Integer.toHexString(temperary).toCharArray(), numf);
				}
				else
				{
					
				}
			}
			else if(line.charAt(0)=='T')
			{
				int lenofline=0;
				lenofline=Integer.parseInt(line.substring(7,9),16);
				rMgr.setMemory(locctr, line.substring(9).toCharArray(), lenofline);
				locctr+=lenofline;
			}
		}
	};

}
