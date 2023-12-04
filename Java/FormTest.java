import java.awt.BorderLayout;
import java.awt.FlowLayout;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.JProgressBar;
import javax.swing.JPanel;

public class FormTest {
    public static void main(String[] args) {
        JFrame frame = new JFrame("フォーム");
        JPanel panel1 = new JPanel(new FlowLayout());
        JPanel panel2 = new JPanel(new FlowLayout());

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLayout(new FlowLayout(FlowLayout.LEFT));

        panel1.add(new LabelWithLength("ユーザー名:"), FlowLayout.LEFT);
        WordCount nameField = new WordCount("exampletaro");
        panel1.add(nameField);

        panel2.add(new LabelWithLength("パスワード:"), FlowLayout.LEFT);
        WordCount passwordField = new WordCount("password");
        panel2.add(passwordField);

        frame.add(panel1);
        frame.add(panel2);
        frame.add(new JButton("送信"));

        frame.pack();
        frame.setVisible(true);

        while(true) {
            nameField.progressBar.setValue(nameField.textField.getText().length());
            passwordField.progressBar.setValue(passwordField.textField.getText().length());
        }
    }
}

class WordCount extends JPanel {
    public JProgressBar progressBar = new JProgressBar();
    public JTextField textField;
    public WordCount(String stakeholder) {
        super();
        textField = new JTextField(stakeholder, 20);
        setLayout(new BorderLayout());
        add(textField, BorderLayout.NORTH);
        add(progressBar, BorderLayout.SOUTH);
    }
}

class LabelWithLength extends JPanel {
    public LabelWithLength(String text) {
        super();
        setLayout(new BorderLayout());
        add(new JLabel(text), BorderLayout.NORTH);
        add(new JLabel("長さ:"), BorderLayout.SOUTH);
    }
}