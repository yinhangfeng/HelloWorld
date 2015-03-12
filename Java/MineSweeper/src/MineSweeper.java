import java.awt.event.*;
import javax.swing.*;
import java.awt.*;
import java.io.*;

public class MineSweeper extends JFrame implements Runnable {
	static MineSweeper mainFrame;
	static Thread t;
	private boolean r=true;
	private Options op = new Options();
	private JLabel[] jl = { new JLabel("ʱ��"), new JLabel(), new JLabel("ʣ������"),
			new JLabel() };
	private JPanel jp1 = new JPanel();
	private JPanel jp2 = new JPanel();
	private JMenuBar mb = new JMenuBar();
	private JMenu gameMenu = new JMenu("��Ϸ");
	private JMenu helpMenu = new JMenu("����");
	private JMenuItem[] game = { new JMenuItem("����Ϸ"), new JMenuItem("ѡ��"),
			new JMenuItem("������Ϸ"), new JMenuItem("��ȡ��Ϸ"), new JMenuItem("�˳�") };
	private JMenuItem help1 = new JMenuItem("����");
	private JMenuItem help2 = new JMenuItem("����");

	private MouseAdapter ma = new MouseAdapter() { // ��������¼�������
		public void mouseClicked(MouseEvent e) {
			if(r)
			{
				t.resume();
				r=false;
			}
			if (e.getButton() == MouseEvent.BUTTON1) 
			{	
				Data.f((Block) e.getSource());
			}
			else if(e.getButton() == MouseEvent.BUTTON2)
			{
				Data.g((Block) e.getSource());
			}
			else if (e.getButton() == MouseEvent.BUTTON3) 
			{
				((Block) e.getSource()).setMark();
				jl[3].setText("" + Data.rMine);
			}		
		}
	};

	public MineSweeper() {
		game[0].addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				r=true;
				Data.newGame();
				newJP1();
			}
		});

		game[1].addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				
				op.setVisible(true);
				op.setLocationRelativeTo(mainFrame);
			}
		});
		game[2].addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				Data.save();
				JOptionPane.showMessageDialog(mainFrame, "����ɹ�", "�浵",
						JOptionPane.INFORMATION_MESSAGE);
			}
		});
		game[3].addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (!new File("save").exists())
					JOptionPane.showMessageDialog(mainFrame, "û�д浵", "����",
							JOptionPane.ERROR_MESSAGE);
				r=true;
				Data.load();
				newJP1();
			}
		});
		game[4].addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				System.exit(0);
			}
		});
		help1.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JOptionPane.showMessageDialog(mainFrame, "�����Windowsɨ��",
						"����", JOptionPane.INFORMATION_MESSAGE);
			}
		});
		help2.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JOptionPane.showMessageDialog(mainFrame,
						"ɨ��ɽկ��1.0\n����: �󺼷�  ������  ������  �˼���", "����",
						JOptionPane.INFORMATION_MESSAGE);
			}
		});
		for (int i = 0; i < game.length; i++) {
			gameMenu.add(game[i]);
		}
		helpMenu.add(help1);
		helpMenu.add(help2);
		mb.add(gameMenu);
		mb.add(helpMenu);
		setJMenuBar(mb);
		for (int i = 0; i < jl.length; i++) {
			jp2.add(jl[i]);
		}
		setLayout(new BorderLayout());
		add(jp1, BorderLayout.CENTER);
		add(jp2, BorderLayout.SOUTH);
		setResizable(false);
		setIconImage(new ImageIcon("Icon/_mine.png").getImage());
		Data.newGame();
		newJP1();
	}

	void newJP1() // �ؽ��������
	{
		jp1.removeAll();
		jp1.setLayout(new GridLayout(Data.high, Data.wide));
		for (int i = 0; i < Data.high; i++)
			for (int j = 0; j < Data.wide; j++) {
				Data.data[i][j].addMouseListener(ma);
				jp1.add(Data.data[i][j]);
			}
		jl[1].setText(""+Data.time);
		jl[3].setText("" + Data.rMine);
		pack();
	}

	public void run() // ʱ���߳�
	{
		while (true) {
			if(r)
				t.suspend();
			jl[1].setText("" + (Data.time++));
			try {
				Thread.sleep(1000);
			} catch (InterruptedException a) {
			}
		}
	}
	

	

	public static void main(String[] args) // ������
	{
		mainFrame = new MineSweeper();
		mainFrame.setTitle("ɨ��");
		mainFrame.setLocationRelativeTo(null);
		mainFrame.setVisible(true);
		mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		t=new Thread(mainFrame);
		t.start();
	}

}

class Options extends JFrame // ѡ����
{
	private JPanel jp1 = new JPanel(), jp2 = new JPanel(), jp3 = new JPanel();
	private JRadioButton[] rb = { new JRadioButton("����10����9x9 ����"),
			new JRadioButton("�м�40����16x16 ����"),
			new JRadioButton("�߼�99����16x30 ����"), new JRadioButton("�Զ���") };
	private ButtonGroup bg = new ButtonGroup();
	private JLabel[] jl = { new JLabel("�߶�(9-24):"), new JLabel("���(9-30):"),
			new JLabel("����(10-719):") };
	private JTextField[] jt = { new JTextField(5), new JTextField(5),
			new JTextField(5) };
	private JButton jb1 = new JButton("ȷ��"), jb2 = new JButton("ȡ��");

	public Options() {
		super("ѡ��");
		rb[Data.difficulty].setSelected(true);
		jt[0].setText("" + Data.high);
		jt[1].setText("" + Data.wide);
		jt[2].setText("" + Data.totalMine);
		jp1.setLayout(new GridLayout(4, 1));
		jp2.setLayout(new FlowLayout());

		ItemListener il = new ItemListener() {
			public void itemStateChanged(ItemEvent e) {
				if (rb[3].isSelected())
					for (int i = 0; i < 3; i++)
						jt[i].setEnabled(true);
				else
					for (int i = 0; i < 3; i++)
						jt[i].setEnabled(false);
			}
		};

		for (int i = 0; i < rb.length; i++) {
			rb[i].addItemListener(il);
			bg.add(rb[i]);
			jp1.add(rb[i]);
		}

		for (int i = 0; i < 3; i++) {
			if (Data.difficulty != 3)
				jt[i].setEnabled(false);
			jp2.add(jl[i]);
			jp2.add(jt[i]);
		}

		jb1.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (rb[0].isSelected()) {
					Data.difficulty = 0;
					Data.high = 9;
					Data.wide = 9;
					Data.totalMine = 10;
				} else if (rb[1].isSelected()) {
					Data.difficulty = 1;
					Data.high = 16;
					Data.wide = 16;
					Data.totalMine = 40;
				} else if (rb[2].isSelected()) {
					Data.difficulty = 2;
					Data.high = 16;
					Data.wide = 30;
					Data.totalMine = 99;
				} else {

					try {
						int df = 3, hi = Integer.parseInt(jt[0].getText()), wi = Integer
								.parseInt(jt[1].getText()), to = Integer
								.parseInt(jt[2].getText());
						if (hi < 9 || hi > 24 || wi < 9 || wi > 30 || to < 10
								|| to > wi*hi- 1) {
							JOptionPane.showMessageDialog(
									MineSweeper.mainFrame, "�Զ������������Χ", "����",
									JOptionPane.ERROR_MESSAGE);
							return;
						}
						Data.difficulty = df;
						Data.high = hi;
						Data.wide = wi;
						Data.totalMine = to;

					} catch (RuntimeException eee) {
						JOptionPane.showMessageDialog(
								MineSweeper.mainFrame, "�Զ����������", "����",
								JOptionPane.ERROR_MESSAGE);return;
					}

				}
				setVisible(false);
				
				Data.newGame();
				MineSweeper.mainFrame.newJP1();
			}
		});

		jb2.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				setVisible(false);
				
			}
		});
		jp3.add(jb1);
		jp3.add(jb2);
		setLayout(new BorderLayout());
		add(jp1, BorderLayout.WEST);
		add(jp2);
		add(jp3, BorderLayout.SOUTH);
		setSize(280, 270);
		setResizable(false);
	}
}

class GameOver extends JFrame // ��Ϸ����ѡ����
{
	private JPanel jp1 = new JPanel(), jp2 = new JPanel();
	private JLabel jl = new JLabel();
	private JButton jb1 = new JButton("����һ��"), jb2 = new JButton("�˳�");

	GameOver() {
		super("��Ϸ����");
		jb1.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				setVisible(false);				
				Data.newGame();
				MineSweeper.mainFrame.newJP1();
			}
		});

		jb2.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				System.exit(0);
			}
		});

		setLayout(new BorderLayout());
		jp1.add(jl);
		jp2.add(jb1);
		jp2.add(jb2);
		add(jp1, BorderLayout.CENTER);
		add(jp2, BorderLayout.SOUTH);
		setSize(200, 100);
		setResizable(false);
	}

	void win() {
		jl.setText("��Ӯ��! ��Ϸʱ�� " + Data.time + "S");
		setLocationRelativeTo(MineSweeper.mainFrame);
		setVisible(true);
	}

	void lose() {
		jl.setText("������");
		setLocationRelativeTo(MineSweeper.mainFrame);
		setVisible(true);
	}
}

class Data // ��Ϸ����
{
	static int totalMine = 40; // �ܵ�����
	static int wide = 16; // ����
	static int high = 16; // ����
	static int difficulty = 1; // �Ѷ�
	static int rMine; // ʣ������
	static int time; // ��Ϸʱ��
	static int coverBlock;// ʣ�า�Ƿ�����
	static Block[][] data = new Block[24][30];
	private static GameOver go = new GameOver();

	static void newGame() { // ����Ϸ
		int n = wide * high;
		int m = totalMine;
		time = 0;
		rMine = totalMine;
		coverBlock = n;
		for (int i = 0; i < high; i++)
			for (int j = 0; j < wide; j++)
				data[i][j] = new Block(i, j);
		// ���õ��ײ����Ե���Ϊ���ĵľŹ�����������1
		for (int i = 0; i < high; i++)
			for (int j = 0; j < wide; j++) {
				if (Math.random() * n < m) {
					m--;
					data[i][j].mine = true;
					for (int a = i - 1; a < i + 2; a++)
						for (int b = j - 1; b < j + 2; b++)
							if (a >= 0 && b >= 0 && a < high && b < wide)
								data[a][b].number++;
				}
				n--;
			}
	}

	static void removeAllCover() // ��Ϸ����ʱ�Ƴ����и���
	{
		for (int i = 0; i < high; i++)
			for (int j = 0; j < wide; j++)
				data[i][j].removeCover();
	}

	static void Recursive(int i, int j) // ���׵ݹ�
	{
		if (data[i][j].mark)
			return;
		data[i][j].removeCover();
		coverBlock--;
		if (data[i][j].number == 0) {
			if (i - 1 >= 0 && j - 1 >= 0 && data[i - 1][j - 1].cover)
				Recursive(i - 1, j - 1);
			if (i - 1 >= 0 && data[i - 1][j].cover)
				Recursive(i - 1, j);
			if (i - 1 >= 0 && j + 1 < wide && data[i - 1][j + 1].cover)
				Recursive(i - 1, j + 1);
			if (j - 1 >= 0 && data[i][j - 1].cover)
				Recursive(i, j - 1);
			if (j + 1 < wide && data[i][j + 1].cover)
				Recursive(i, j + 1);
			if (i + 1 < high && j - 1 >= 0 && data[i + 1][j - 1].cover)
				Recursive(i + 1, j - 1);
			if (i + 1 < high && data[i + 1][j].cover)
				Recursive(i + 1, j);
			if (i + 1 < high && j + 1 < wide && data[i + 1][j + 1].cover)
				Recursive(i + 1, j + 1);

		}
	}

	static void save() {
		DataOutputStream outFile = null;

		try {
			outFile = new DataOutputStream(new BufferedOutputStream(
					new FileOutputStream("save")));
			outFile.writeInt(Data.totalMine);
			outFile.writeInt(Data.wide);
			outFile.writeInt(Data.high);
			outFile.writeInt(Data.difficulty);
			outFile.writeInt(Data.rMine);
			outFile.writeInt(Data.time);
			outFile.writeInt(Data.coverBlock);
			for (int i = 0; i < Data.high; i++)
				for (int j = 0; j < Data.wide; j++) {
					outFile.writeInt(Data.data[i][j].number);
					outFile.writeBoolean(Data.data[i][j].mine);
					outFile.writeBoolean(Data.data[i][j].cover);
					outFile.writeBoolean(Data.data[i][j].mark);
				}

		} catch (IOException e) {
		} finally {
			try {
				if (outFile != null)
					outFile.close();
			} catch (IOException e) {
			}
		}

	}

	static void load() {
		DataInputStream inFile = null;
		try {
			inFile = new DataInputStream(new BufferedInputStream(
					new FileInputStream("save")));
			totalMine = inFile.readInt();
			wide = inFile.readInt();
			high = inFile.readInt();
			difficulty = inFile.readInt();
			rMine = inFile.readInt();
			time = inFile.readInt();
			coverBlock = inFile.readInt();
			for (int i = 0; i < high; i++)
				for (int j = 0; j < wide; j++)
					data[i][j] = new Block(i, j, inFile.readInt(),
							inFile.readBoolean(), inFile.readBoolean(),
							inFile.readBoolean());
		} catch (IOException e) {
		} finally {
			try {
				if (inFile != null)
					inFile.close();
			} catch (IOException e) {
			}
		}

	}

	static void f(Block blo) // �ж���Ϸ�Ƿ����
	{
		int i,j;
		i=blo.i;
		j=blo.j;
		if (data[i][j].cover && !data[i][j].mark && !data[i][j].mine) {
			Recursive(i, j);
			if (coverBlock == totalMine) {
				removeAllCover();
				go.win();
			}
		} else if (data[i][j].cover && !data[i][j].mark && data[i][j].mine) {
			removeAllCover();
			go.lose();
		}
		
	}
	
	static void g(Block blo)
	{
		int i,j;
		int nmark=0;
		if(blo.cover)
			return;
		i=blo.i;
		j=blo.j;
		for (int a = i - 1; a < i + 2; a++)
			for (int b = j - 1; b < j + 2; b++)
				if (a >= 0 && b >= 0 && a < high && b < wide&&data[a][b].mark)
					nmark++;
		if(nmark!=blo.number)
			return;
		for (int a = i - 1; a < i + 2; a++)
			for (int b = j - 1; b < j + 2; b++)
				if (a >= 0 && b >= 0 && a < high && b < wide)
					f(data[a][b]);		
	}

}

class Block extends JButton // ������
{

	int i;
	int j; // ��������
	int number = 0; // �Է���Ϊ���ĵľŹ����еĵ�����
	boolean mine = false; // �����Ƿ�Ϊ����
	boolean cover = true; // �����Ƿ񱻸���
	boolean mark = false; // �����Ƿ񱻱��
	private static ImageIcon coverIcon = new ImageIcon("Icon/cover.png");
	private static ImageIcon markIcon = new ImageIcon("Icon/mark.png");
	private static ImageIcon _coverIcon = new ImageIcon("Icon/notcover.png");
	private static ImageIcon mineIcon = new ImageIcon("Icon/mine.png");
	private static Insets in = new Insets(-2, -2, -2, -2);

	Block(int a, int b) { // ����Ϸ���캯��
		i = a;
		j = b;
		setIcon(coverIcon);
		setMargin(in);
		setHorizontalTextPosition(SwingConstants.CENTER);
	}

	Block(int a, int b, int n, boolean m, boolean c, boolean ma) { // ��ȡ��Ϸ���캯��
		i = a;
		j = b;
		number = n;
		mine = m;
		cover = c;
		mark = ma;
		if (mark)
			setIcon(markIcon);
		else if (cover)
			setIcon(coverIcon);
		else {
			setIcon(_coverIcon);
			if (number != 0)
				setText("" + number);
		}
		setMargin(in);
		setHorizontalTextPosition(SwingConstants.CENTER);
	}

	void removeCover() // �Ƴ����Ǻ���
	{
		cover = false;
		if (mine)
			setIcon(mineIcon);
		else {
			setIcon(_coverIcon);
			if (number != 0)
				setText("" + number);
		}
	}

	void setMark() // ���ñ�Ǻ���
	{
		if (cover) {
			mark = !mark;
			if (mark) {
				setIcon(markIcon);
				Data.rMine--;
			} else {
				setIcon(coverIcon);
				Data.rMine++;
			}
		}
	}

}
