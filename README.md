![Smousie_title](https://github.com/user-attachments/assets/7a7c978f-05c3-4df5-8fd9-e708c464de26)

**Smousie** is an arcade game inspired by **Puzzle Bobble** where a mouse is in charge of collecting fruits to make smoothies and sell them. 
Use the pointer to pop the fruits on the screen and get the highest score possible before they reach the bottom!

Play with your **mouse**, hold down **left button** to direct the pointer to the mouse position and release it to launch the fruit.

![Smousie_gameplay2](https://github.com/user-attachments/assets/52735c00-1adc-4d02-8029-19f930481e6d)

## Challenges
The biggest challenge in recreating the original game was to understand how I could arrange the fruits on the screen to make it easier to pop and drop them as optimally as possible. 
I decided to implement this by creating a `multidimensional array` for the available screen fruit spaces and use certain functions that go through it changing the values inside each of this space. 
You can see the full implementation on `GameManager`.
