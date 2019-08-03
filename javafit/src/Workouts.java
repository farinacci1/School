import java.util.ArrayList; // We choose ArrayList over Vector because...?

public class Workouts {
	private final ArrayList<Workout> workoutList = new ArrayList<Workout>();

	public final void addWorkout(String name, Config.Equipment equipment, Config.Muscle primaryMuscle,
			Config.Muscle secondaryMuscle, String desc, String reminders) {
		Workout newWorkout = new Workout(name, equipment, primaryMuscle, secondaryMuscle, desc, reminders);
		workoutList.add(newWorkout);
	}

	public final void addWorkout(Workout workout) {
		workoutList.add(workout);
	}

	public final Workouts getWorkoutsByMuscle(Config.Muscle m, boolean includeSecondary) {
		Workouts retval = new Workouts();
		for (Workout w : workoutList) {
			if (w.hasPrimaryMuscle(m)) {
				retval.addWorkout(w);
			} else if (includeSecondary && w.hasSecondaryMuscle(m)) { // Why did I put the includeSecondary first?
				retval.addWorkout(w);
			}
		}
		return retval;
	}

	public final Workouts getWorkoutsByMuscle(ArrayList<Config.Muscle> m, boolean includeSecondary) {
		Workouts retval = new Workouts();
		for (Config.Muscle muscle : m) {
			retval.workoutList.addAll(getWorkoutsByMuscle(muscle, includeSecondary).workoutList);

		}
		return retval;
	}

	public final Workouts getWorkoutsByEquipment(Config.Equipment e) {
		Workouts retval = new Workouts();
		for (Workout w : workoutList) {
			if (w.hasEquipment(e)) {
				retval.addWorkout(w);
			}
		}
		return retval;
	}

	public final Workouts getWorkoutsByEquipment(ArrayList<Config.Equipment> e) {
		Workouts retval = new Workouts();
		for (Config.Equipment equip : e) {
			retval.workoutList.addAll(getWorkoutsByEquipment(equip).workoutList);

		}
		return retval;
	}

	public final ArrayList<String> getNames() {
		ArrayList<String> retval = new ArrayList<String>();
		for (Workout w : workoutList) {
			retval.add(w.getName());
		}
		return retval;
	}
	public final ArrayList<String> getEquipment() {
		ArrayList<String> retval = new ArrayList<String>();
		for (Workout w : workoutList) {
			retval.add(w.getEquipment());
		}
		return retval;
	}
	public final ArrayList<String[]> getFullInformation() {
		ArrayList<String[]> retval = new ArrayList<String[]>();
		for (Workout w : workoutList) {
			String[] info = new String[6];
			info[0] = w.getName();
			info[1] = w.getEquipment();
			info[2] = w.getPrimaryMuscle();
			info[3] = w.getSecondaryMuscle();
			info[4] = w.getDesc();
			info[5] = w.getReminders();
			retval.add(info);
		}
		return retval;
	}
	public final int size() 
	{
		return this.workoutList.size();
	}
	public final Workout getWorkout(int index) 
	{
		return this.workoutList.get(index);
	}
	public final String getWorkoutEquipment(int index) 
	{
		return this.workoutList.get(index).getEquipment();
	}
	public final int findWorkoutByName(String name)
	{
		for (Workout workout: this.workoutList) 
		{
			if(workout.getName().equals(name)) 
			{
				return workoutList.indexOf(workout);
			}
		}
		return -1;
	}
	
}