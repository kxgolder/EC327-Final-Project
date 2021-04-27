# EC327-Final-Project

The Wellness Tracker App

In order to run the App, an additional library may need to be added. It is called Boost and called by installed on Ubuntu using the following command:
sudo apt-get install libboost-all-dev

If all the files are included in the working directory (including all the images, "Buttoon.h", and the "Makefile"), you can compile the app by running the command:
make Wellness_App

After compiling the program, you can run the App by typing in the command:
wa

Once you have the App open, the calander will be set to the current day and the following 6 days of the week.

In the app you can do the following:

1) Change your desired daily water-intake (in cups) by clicking on the settings button in the bottom right hand corner of the screen. Once the popup window comes up, type in the number of cups you would like to drink that day and hit, the 'return' key. Your new goal will be updated and displayed on the right, bottom screen. If you have water-intake inputs already, everything will be updated.

2) Add how many cups you have drinken for the day. Do this by clicking on the water botton to the right of the pink rectangle. Once the popup window comes up, type in the number of cups you would like to add to your daily water-tracker, and hit the 'return' key. The water tracker will visually update and get fuller, along with a percentage of water you have drinken to acheive you goal.

3) If you would like to clear your water-tracker, you can hit the 'reset' button unter the button to add water on the right hand side of the window. the water-tracker will reset, and you percentage of your goal will return to 0.

4) Input events by clicking on the 'plus sign' on the left hand, bottom screen. Once the blue cursor shows up, you may start typing in the date of your event. Look at the example for how to input the date correctly. Input your date and hit the 'return' key. Don't worry -- if your date is invalid, an error message will pop up and you may re-enter and new date. Close the error messages by clicknig on the red 'X' button. Once you input a valid date, you may now input the time and hit the 'return' key. If your time is valid, you may enter the event name, and hit the 'return' key. After inputting a valid date, time, and event, you will be asked to confirm it. After you confirm, the event will be displayed on the corresponding date.

5) You have delete events by right-clicking on the event box shown in the calander. You will receive a pop-up message asking you to confirm the event deletion.

6) You can scroll through adjacent weeks by pressing on the arrows to the left and right of the calander at the top of the screen. All your stored inputs will be displayed if the responding days are shown.


Once your information is added, you will receive reminders based upon your events and you current-water intake! If you are on the screen, reminder messages will sparatically (every hour) come up to help you acheive your water goal and check up on your wellness! Every morning at 8am, you will also be reminded on how many events you have for the day if you have any.

When closing the App, your events and water-intake goal will be saved for the next time you open it.

Enjoy, and stay heathy!