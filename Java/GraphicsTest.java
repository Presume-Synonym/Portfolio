package lecture8;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.WindowConstants;

public class GraphicsTest {

    public static void main(String[] args) {
        JFrame window = new JFrame("グラフィックスのテスト");
        window.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);

        window.add(new Panel4Graphics());

        window.setSize(600, 400);
        window.setVisible(true);
    }
}

class Panel4Graphics extends JPanel {
    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        Graphics2D graphics2d = (Graphics2D)g;

        graphics2d.drawString("Hello Swing World.", 150, 100);
        graphics2d.drawLine(10, 10, 200, 150);
        graphics2d.drawRect(30, 50, 130, 200);
        graphics2d.drawOval(10, 10, 300, 200);
        graphics2d.drawArc(50, 50, 10, 200, 0, 270);

        graphics2d.setColor(Color.BLUE);
        graphics2d.fillRect(350, 20, 100, 50);
        graphics2d.fillOval(350, 110, 100, 50);
        graphics2d.fillArc(350, 200, 100, 100, 30, 300);
    }
}