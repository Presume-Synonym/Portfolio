import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.RoundRectangle2D;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.WindowConstants;
import javax.swing.border.Border;
import javax.swing.border.LineBorder;

public class Hatten extends JFrame implements ActionListener, MouseListener {
    private JLabel label;
    private JPanel panel;
    private ColoredButton button1;
    private ColoredButton button2;
    private ColoredButton button3;

    public ButtonTest() {
        super("発展");
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        setSize(400, 300);
        setLayout(new BorderLayout());

        label = new JLabel("ここが変わります");
        panel = new JPanel();
        panel.setLayout(new FlowLayout(FlowLayout.LEFT));
        button1 = new ColoredButton("ボタン1");
        button2 = new ColoredButton("ボタン2");
        button3 = new ColoredButton("ボタン3");
        button1.setPreferredSize(new Dimension(100, 30));
        button2.setPreferredSize(new Dimension(100, 30));
        button3.setPreferredSize(new Dimension(100, 30));
        add(label, BorderLayout.NORTH);
        panel.add(button1);
        panel.add(button2);
        panel.add(button3);
        panel.setBackground(Color.white);
        add(panel, BorderLayout.CENTER);

        button1.addActionListener(this);
        button2.addActionListener(this);
        button3.addActionListener(this);
        addMouseListener(this);
        getContentPane().setBackground(Color.white);
    }

    @Override
    public void actionPerformed(ActionEvent event) {
        if (event.getSource() == button1) {
            label.setText("ボタン1が押されました");
        } else if (event.getSource() == button2) {
            label.setText("ボタン2が押されました");
        } else if (event.getSource() == button3) {
            label.setText("ボタン3が押されました");
        }
    }

    @Override
    public void mouseClicked(MouseEvent event) {
    }

    @Override
    public void mousePressed(MouseEvent event) {
        label.setText("x: " + event.getX() + ", y: " + event.getY());
    }

    @Override
    public void mouseReleased(MouseEvent event) {
    }

    @Override
    public void mouseEntered(MouseEvent event) {
    }

    @Override
    public void mouseExited(MouseEvent event) {
    }

    public static void main(String[] args) {
        JFrame frame = new ButtonTest();
        frame.setVisible(true);
    }
}


class ColoredButton extends JButton implements MouseListener {
    Color originalColor;

    public ColoredButton(String name) {
        super(name);
        addMouseListener(this);
        setContentAreaFilled(false);
        setBackground(Color.decode("#d6e6f5"));
        originalColor = getBackground();
    }

    @Override
    protected void paintComponent(Graphics g) {
        if (getModel().isArmed()) {
            g.setColor(Color.lightGray);
        } else {
            g.setColor(getBackground());
        }

        Graphics2D g2 = (Graphics2D) g;
        int arc = 10;
        g2.fill(new RoundRectangle2D.Double(0, 0, getWidth() - 1, getHeight() - 1, arc, arc));

        super.paintComponent(g);
    }

    @Override
    protected void paintBorder(Graphics g) {
        g.setColor(getForeground());
        Graphics2D g2 = (Graphics2D) g;
        int arc = 10;
        g2.draw(new RoundRectangle2D.Double(0, 0, getWidth() - 1, getHeight() - 1, arc, arc));
    }

    @Override
    public void setBorder(Border border) {
        border = new LineBorder(Color.lightGray);
        super.setBorder(border);
    }

    @Override
    public void mouseClicked(MouseEvent event) {
    }

    @Override
    public void mousePressed(MouseEvent event) {
    }

    @Override
    public void mouseReleased(MouseEvent event) {
    }

    @Override
    public void mouseEntered(MouseEvent event) {
        setBackground(Color.red);
        repaint();
    }

    @Override
    public void mouseExited(MouseEvent event) {
        setBackground(originalColor);
        repaint();
    }
}