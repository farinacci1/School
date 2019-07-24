import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;



// Write code to load the workouts from the provided workouts.csv file. The function should return a Workouts object.

public class FileAccess {
  
  public static Workouts loadWorkouts() {
	
    // What is a try/catch block and why do we need one?
    // What is an exception?
	File file = new File(Config.WORKOUTFILE);
	
	Workouts workouts = new Workouts();
	try 
	{
		Scanner scan = new Scanner(file);
		String line = "";
		while( scan.hasNextLine()) 
		{
			line = scan.nextLine();
			String tokens[] = line.split(",");
			String name = tokens[0];
			Workouts.Equipment equipment = Workouts.Equipment.valueOf(tokens[1]);
			Workouts.muscle primaryMuscle = Workouts.muscle.valueOf(tokens[2]);
			Workouts.muscle secondaryMuscle = Workouts.muscle.valueOf(tokens[3]);
			String desc = tokens[4];
			String reminder = tokens[5];
			
			workouts.addWorkout(name, equipment,  primaryMuscle,  secondaryMuscle,  desc,  reminder);
		}
		scan.close();
	} 
	catch (FileNotFoundException e) 
	{
		e.printStackTrace();
	}

	
	return workouts;
  }

}
