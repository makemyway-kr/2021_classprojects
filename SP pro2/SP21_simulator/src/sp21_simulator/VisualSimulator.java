package sp21_simulator;
import java.awt.*;
import java.awt.event.ActionListener;

import javax.swing.*;
import java.io.File;

/**
 * VisualSimulator는 사용자와의 상호작용을 담당한다. 즉, 버튼 클릭등의 이벤트를 전달하고 그에 따른 결과값을 화면에 업데이트
 * 하는 역할을 수행한다.
 * 
 * 실제적인 작업은 SicSimulator에서 수행하도록 구현한다.
 */

@SuppressWarnings("serial")
public class VisualSimulator extends JFrame {
	ResourceManager resourceManager = new ResourceManager();
	SicLoader sicLoader = new SicLoader(resourceManager);
	SicSimulator sicSimulator = new SicSimulator(resourceManager);
	static JFrame fr;
	
	/**
	 * 프로그램 로드 명령을 전달한다.
	 */
	public void load(File program) {
		// ...
		sicLoader.load(program);
		sicSimulator.load(program);
	};

	/**
	 * 하나의 명령어만 수행할 것을 SicSimulator에 요청한다.
	 */
	public void oneStep() {

	};

	/**
	 * 남아있는 모든 명령어를 수행할 것을 SicSimulator에 요청한다.
	 */
	public void allStep() {

	};

	/**
	 * 화면을 최신값으로 갱신하는 역할을 수행한다.
	 */
	public void update() {

	};

	public static void main(String[] args) {
		fr=new JFrame("SP21 pro2 20192698");
		fr.setSize(700,300);
		fr.setVisible(true);
		fr.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		Container contentpane=fr.getContentPane();
	}
}