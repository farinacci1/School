# Summer 314 Project, Part 1

Overview:
This project is composed of two parts. This is phase 1. It is due on Wednesday, July 24th. Thereafter, you will get one more phase. Read the comments in the code. These comments will give you hints on how to create the code you need. There are also questions in the comments in the first phase. Answering these questions is worth points.

Github:
Part of this assignment is to create a gitub account (if you don't have one already) and clone this project to your github account. Use your github account to backup your code. When you turn in your project, make sure you have committed the most recent version of your code to your github account. This github account will be used in grading your assignment.

Set your github to private. This is at the bottom of the "Options"
Set "Glen-TAMU" as a collaborator, so I can grade your work.

Intro to Github: https://guides.github.com/activities/hello-world/

Cloning a repo: https://help.github.com/en/articles/cloning-a-repository

Coding:
Your job is to extend the functionality of the provided code. Do not delete or alter any of the given code. Your code must have the same return values and function prototypes as what is provided, as well as the same level of information hiding and encapsulation.

You have two parts to this phase of the project. The first is to create an object to embody the concept of “Workouts.”  The data values you need for a workout are already in the skeleton file. You will need to write code to handle the expected methods of the Workouts class. You will need to write helper methods to perform these tasks.

The second part of this phase is to write a file reader that will load the workouts from a CSV (comma separated values) file into the Workouts object. You can assume that there are no extra commas in the CSV file that need to be escaped. All commas act as separators. Do not change the path in the config class. 

The main javaFit file has a series of function calls that will be used to test the performance of your code. Do not modify these. Looking at these lines of code will give you some insight into how your code will be used.

You will want to test your code to ensure it is both correct and highyl functional. The CSV file is very short in this phase, so you can figure out what a function call should return by hand to validate your code. When returning multiple values, duplicates should be removed. 

Style:
In programming, it is very important to use a specific and consistent programming style. Use the refactor tool to rename class names as per the Google Java Style Guide. https://google.github.io/styleguide/javaguide.html when in doubt, use this style guide.

Notes:
Indents in these files are set to 2 spaces. You can change this setting in your Eclipse file by clicking Window Preferences. -> Expand Java Code Style ->Click Formatter -> Click the Edit button -> Click the Indentation tab.
Under General Settings, set Tab policy to Spaces only, set spaces to 2.

You are going to need to load files in this app. Eclipse, however, is going to be picky about where the “current directory” is. We could solve this by using absolute paths, but that means you may run into problems when you are trying to find that location on your system, which may be laid out differently than my system. So we will instead set up the Eclipse working directory

Go to the "run configuration", open the "Arguments" (second) tab, and click the "Other" radio button. Then enter an absolute pathname as the working directory for the launched application. This directory will be the one where you put your csv files. Mine are, cleverly, in “C:\Users\Glen\eclipse-workspace\javafit\data”. Click Apply.

Hints:
1. It is normal to temporarily comment ou code so you can test the functionality of your code piecemeal. Just remember to uncomment your code before turning it in. 
2. One of the key aspects of Java is platform independence. Make sure you never hard-code your program to only work on one operating system or in one specific computer.
3. Always check the online Java documentation. Many things you want to do are already included as classes in java. Import and use them rather than reinventing the wheel.
4. Make sure you start a new project in eclipse for this assignment. A common mistake people make is to import the code into an existing project and then have trouble sperating old code from the assignment.

What You Turn In:
1) The completed .java files. Include all four.
2) A text file with your answers to the questions in the code comments.
3) Your github repo address. Include this at the top of your text file. Remember to set your repo private and add Glen-TAMU as a collaborator.

Grading: 
Grading will be based upon completeness. If your code is fully functional and passes all tests while adhering to the design guidelines, you get full points for this section. If your code is partially functional, you will get partial credit. If your code does not compile or run, you will get minimal credit. If you turn nothing in, you will get no credit.

Academic Honesty: As with most programming project, student code will be subjected to extensive anti-cheating monitoring. Algorithm design will be checked, as well as code layout. 

Future Phases and Late Work:
A solution to each phase will be provided, as well as new start files, so that a student who does poorly in one phase is not starting at a disadvantage in the following section. Students may continue to use their own code, or they may use the provided start files for each subsequent phase.

Because solutions will be provided, late assignments can not be accepted.
