import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;

/**
 * @desc Project Phase 1. Do not change this file. Your code should work flawlessly with this file when completed.
 * @author Glen
 *
 */

public class javaFit {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		
	  // Load data into app
	  Workouts workouts= FileAccess.loadWorkouts();

	  // Print out all the names of workouts that work a specific primary muscle
	  // NOTE: You do not need to handle cases where the user makes a typo. It is okay to error on that code, because we will not, in the future, allow direct user input like this. 
	  System.out.println("Enter one of the following: ABS, BACK, BICEPS, CHEST, FOREARM, GLUTES, LOWERLEG, SHOULDER, TRICEPS, UPPERLEG");
	  Scanner scanner = new Scanner(System. in);
    String muscle = scanner.nextLine();
    System.out.println(workouts.getWorkoutsByMuscle(Workouts.muscle.valueOf(muscle), false).getNames()); // Pay special attention to how we convert this string to an enum value!
	  
	  // Print out the full information on workouts that use a specific equipment and work a muscle (primary or secondary)
    System.out.println("\nEnter one of the following: ABS, BACK, BICEPS, CHEST, FOREARM, GLUTES, LOWERLEG, SHOULDER, TRICEPS, UPPERLEG");
    String muscleAgain = scanner.nextLine();
    System.out.println("Enter one of the following: BARBELL, BODYWEIGHT, DUMBBELL, CABLE, HAMMERSTRENGTH");
    String equipment = scanner.nextLine();
    Workouts chosenWorkouts =workouts.getWorkoutsByMuscle(Workouts.muscle.valueOf(muscleAgain), true).getWorkoutsByEquipment(Workouts.Equipment.valueOf(equipment)); 
    ArrayList<String[]> workoutData =chosenWorkouts.getFullInformation(); 
    for(String[] workout : workoutData) {
      System.out.println(Arrays.deepToString(workout));
    }
	  
    System.out.println("\nEnter one more of the following: BARBELL, BODYWEIGHT, DUMBBELL, CABLE, HAMMERSTRENGTH");
    String equipment2 = scanner.nextLine();
    ArrayList<Workouts.Equipment> equipmentList = new ArrayList<Workouts.Equipment>();
    equipmentList.add(Workouts.Equipment.valueOf(equipment));
    equipmentList.add(Workouts.Equipment.valueOf(equipment2));
    ArrayList<String[]> refinedWorkoutData =workouts.getWorkoutsByMuscle(Workouts.muscle.valueOf(muscleAgain), true).getWorkoutsByEquipment(equipmentList).getFullInformation(); // Take note of the valueOf() method. You will need this for your code.
    for(String[] workout : refinedWorkoutData) {
      System.out.println(Arrays.deepToString(workout));
    }
    
    scanner.close();	  
	  
	}

}
