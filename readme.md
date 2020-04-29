Breakout 62
===============	

This game is implement from : https://bitbucket.org/01418113/breakout_template_62

How to install
---------------	
First step : use this command on terminal
> make

Second Step : access game via breakout.exe

How to play (Control)
---------------	
- space bar -> use for press next on menu and go back from record page
- up arrow, down arrow -> use for change menu
- ESE or escape -> use for go back to menu page
- left arrow, right arrow -> use for move paddle in game

**special key
- numpad 1 -> use for force to Next Level (use in game)

Feature
---------------	
- There is brick, ball and paddle
- There are speical bricks
    - crack brick -> This brick need ball collide 1 hit before break | give 10 score

    ![](https://raw.githubusercontent.com/bankdevelop/breakout_62/master/asset/img/brick0.png)

    - normal brick -> This brick need ball collide 2 hit before break | give 10 score/per hit

    ![](https://raw.githubusercontent.com/bankdevelop/breakout_62/master/asset/img/brick.png)

    - bonus brick -> This brick is the same as crack brick but **give 30 point

    ![](https://raw.githubusercontent.com/bankdevelop/breakout_62/master/asset/img/brick1.png)

    - blackhold brick -> This brick can break near brick | give 300 point

    ![](https://raw.githubusercontent.com/bankdevelop/breakout_62/master/asset/img/brick2.png)
    
- Ghost
    - When ball hit ghost, you receive 10000 point
- There is 5 level
    - Level 1 have crack brick and normal brick
    - Level 2 add bonus brick , paddle speed and ball speed
    - Level 3-5 add blackhold brick, paddle speed and ball speed
- There is score page for record Best 10 score [Bug on writing File]
