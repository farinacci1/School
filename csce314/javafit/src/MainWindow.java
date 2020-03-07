import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.EnumMap;
import java.util.Map.Entry;
import javax.swing.*;
import javax.swing.border.EmptyBorder;

// Bonus points: Create an icon (or find a public domain icon. Keep in mind federal Copyright law and TAMU's plagiarism policy and add it to the home screen window.
public class MainWindow {
  
  private final JFrame mainFrame = new JFrame(Config.APPLICATIONNAME);
  private final JDialog selectWorkout = new JDialog(mainFrame, "Select Workout");
  private JComboBox<String> cboType, cboGoal;
  private JSpinner spnDuration;
  private final Workouts workouts;
  private final EnumMap<Config.MuscleGroup, ArrayList<Config.Muscle>> muscleGroups;

  MainWindow(Workouts workouts, EnumMap<Config.MuscleGroup, ArrayList<Config.Muscle>> muscleGroups) {
	  this.workouts = workouts;
	  this.muscleGroups = muscleGroups;
	  GridLayout layout = new GridLayout(muscleGroups.size(),1);
	  mainFrame.setLayout(layout);
	  for(Entry<Config.MuscleGroup, ArrayList<Config.Muscle>> entry : muscleGroups.entrySet()) 
	  {
	      String uppercaseFirst = entry.getKey().name().substring(0,1).toUpperCase();
	      String lowercaseRest = entry.getKey().name().substring(1).toLowerCase();
		  JButton button = new JButton(uppercaseFirst + lowercaseRest + " Workout");
		  
		  button.addActionListener(new ActionListener() 
		  {
			@Override
			public void actionPerformed(ActionEvent event) {
				showWorkouts(entry.getValue());		
			}
			  
		  });
		  
		  mainFrame.add(button);
	  }
	  launchHomeScreen();
  }
  
  private void launchHomeScreen() {
	  mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	  mainFrame.setSize(600, 400);
	  mainFrame.setVisible(true);
 
  }
  
  // This is the method your actionlistener should call. It should create and display a WorkoutsPanel.
  private void showWorkouts(ArrayList<Config.Muscle> muscles) {
	  mainFrame.getContentPane().removeAll();
	  mainFrame.repaint();
	  WorkoutsPanel panel = new WorkoutsPanel(muscles, workouts);
	  mainFrame.getContentPane().add(panel);
	  SwingUtilities.updateComponentTreeUI(mainFrame);
	  
  }
  
}
