import java.awt.event.*;
import javax.swing.*;
import java.awt.*;
import java.io.*;

public class MineSweeper extends JFrame implements Runnable {
	static MineSweeper mainFrame;
	static Thread t;
	private boolean r=true;
	private Options op = new Options();
	private JLabel[] jl = { new JLabel("时间"), new JLabel(), new JLabel("剩余雷数"),
			new JLabel() };
	private JPanel jp1 = new JPanel();
	private JPanel jp2 = new JPanel();
	private JMenuBar mb = new JMenuBar();
	private JMenu gameMenu = new JMenu("游戏");
	private JMenu helpMenu = new JMenu("帮助");
	private JMenuItem[] game = { new JMenuItem("新游戏"), new JMenuItem("选项"),
			new JMenuItem("保存游戏"), new JMenuItem("读取游戏"), new JMenuItem("退出") };
	private JMenuItem help1 = new JMenuItem("帮助");
	private JMenuItem help2 = new JMenuItem("关于");

	private MouseAdapter ma = new MouseAdapter() { // 点击方块事件监听器
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
				JOptionPane.showMessageDialog(mainFrame, "保存成功", "存档",
						JOptionPane.INFORMATION_MESSAGE);
			}
		});
		game[3].addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (!new File("save").exists())
					JOptionPane.showMessageDialog(mainFrame, "没有存档", "错误",
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
				JOptionPane.showMessageDialog(mainFrame, "请参照Windows扫雷",
						"帮助", JOptionPane.INFORMATION_MESSAGE);
			}
		});
		help2.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JOptionPane.showMessageDialog(mainFrame,
						"扫雷山寨版1.0\n作者: 殷杭锋  沈利国  俞明辉  潘季逸", "关于",
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

	void newJP1() // 重建雷区面板
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

	public void run() // 时间线程
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
	

	

	public static void main(String[] args) // 主函数
	{
		mainFrame = new MineSweeper();
		mainFrame.setTitle("扫雷");
		mainFrame.setLocationRelativeTo(null);
		mainFrame.setVisible(true);
		mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		t=new Thread(mainFrame);
		t.start();
	}

}

class Options extends JFrame // 选项框架
{
	private JPanel jp1 = new JPanel(), jp2 = new JPanel(), jp3 = new JPanel();
	private JRadioButton[] rb = { new JRadioButton("初级10个雷9x9 网格"),
			new JRadioButton("中级40个雷16x16 网格"),
			new JRadioButton("高级99个雷16x30 网格"), new JRadioButton("自定义") };
	private ButtonGroup bg = new ButtonGroup();
	private JLabel[] jl = { new JLabel("高度(9-24):"), new JLabel("宽度(9-30):"),
			new JLabel("雷数(10-719):") };
	private JTextField[] jt = { new JTextField(5), new JTextField(5),
			new JTextField(5) };
	private JButton jb1 = new JButton("确定"), jb2 = new JButton("取消");

	public Options() {
		super("选项");
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
									MineSweeper.mainFrame, "自定义参数超出范围", "错误",
									JOptionPane.ERROR_MESSAGE);
							return;
						}
						Data.difficulty = df;
						Data.high = hi;
						Data.wide = wi;
						Data.totalMine = to;

					} catch (RuntimeException eee) {
						JOptionPane.showMessageDialog(
								MineSweeper.mainFrame, "自定义参数错误", "错误",
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

class GameOver extends JFrame // 游戏结束选项框架
{
	private JPanel jp1 = new JPanel(), jp2 = new JPanel();
	private JLabel jl = new JLabel();
	private JButton jb1 = new JButton("再玩一局"), jb2 = new JButton("退出");

	GameOver() {
		super("游戏结束");
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
		jl.setText("你赢了! 游戏时间 " + Data.time + "S");
		setLocationRelativeTo(MineSweeper.mainFrame);
		setVisible(true);
	}

	void lose() {
		jl.setText("你输了");
		setLocationRelativeTo(MineSweeper.mainFrame);
		setVisible(true);
	}
}

class Data // 游戏数据
{
	static int totalMine = 40; // 总地雷数
	static int wide = 16; // 列数
	static int high = 16; // 行数
	static int difficulty = 1; // 难度
	static int rMine; // 剩余雷数
	static int time; // 游戏时间
	static int coverBlock;// 剩余覆盖方块数
	static Block[][] data = new Block[24][30];
	private static GameOver go = new GameOver();

	static void newGame() { // 新游戏
		int n = wide * high;
		int m = totalMine;
		time = 0;
		rMine = totalMine;
		coverBlock = n;
		for (int i = 0; i < high; i++)
			for (int j = 0; j < wide; j++)
				data[i][j] = new Block(i, j);
		// 放置地雷并将以地雷为中心的九宫格中雷数加1
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

	static void removeAllCover() // 游戏结束时移除所有覆盖
	{
		for (int i = 0; i < high; i++)
			for (int j = 0; j < wide; j++)
				data[i][j].removeCover();
	}

	static void Recursive(int i, int j) // 挖雷递归
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

	static void f(Block blo) // 判断游戏是否结束
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

class Block extends JButton // 方块类
{

	int i;
	int j; // 方块坐标
	int number = 0; // 以方块为中心的九宫格中的地雷数
	boolean mine = false; // 方块是否为地雷
	boolean cover = true; // 方块是否被覆盖
	boolean mark = false; // 方块是否被标记
	private static ImageIcon coverIcon = new ImageIcon("Icon/cover.png");
	private static ImageIcon markIcon = new ImageIcon("Icon/mark.png");
	private static ImageIcon _coverIcon = new ImageIcon("Icon/notcover.png");
	private static ImageIcon mineIcon = new ImageIcon("Icon/mine.png");
	private static Insets in = new Insets(-2, -2, -2, -2);

	Block(int a, int b) { // 新游戏构造函数
		i = a;
		j = b;
		setIcon(coverIcon);
		setMargin(in);
		setHorizontalTextPosition(SwingConstants.CENTER);
	}

	Block(int a, int b, int n, boolean m, boolean c, boolean ma) { // 读取游戏构造函数
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

	void removeCover() // 移除覆盖函数
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

	void setMark() // 设置标记函数
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
