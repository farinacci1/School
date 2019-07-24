// Write code to load the workouts from the provided workouts.csv file. The function should return a Workouts object.
import Config;
import Workouts
public class FileAccess {
  
  public static Workouts loadWorkouts() {
    // What is a try/catch block and why do we need one?
    // What is an exception?
		File file = new File(WORKOUTFILE);
		Scanner scan = null;
		
			scan  = new Scanner(file);
			while(scan.hasNext()) {
				String line = scan.nextLine();
				String[] tokens = line.split(",");
				String name = tokens[0];
				String equipment = tokens[1];
				String primaryMuscle = tokens[2];
				String secondaryMuscle = tokens[3];
				String desc = tokens[4];
				String reminders = tokens[5];
				
				muscle primary = muscle.valueof(primaryMuscle);
				muscle secondary = muscle.valueof(secondaryMuscle);
				Equipment equip = Equipment.valueof(equipment);
				Workout workout =  new Workout(name,equip,primary,secondary,desc,remain);
				Workouts.addWorkout(workout);
			
			}
			scan.close();
  }

}
