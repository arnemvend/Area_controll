# Area Control
---
### Developed with Unreal Engine 5
---
### https://disk.yandex.ru/d/Ec_2aY_thAa_SA
---
I am creating this project for my portfolio using UE5 and C++. A game in the genre of RTS and TD. The game has a version for PC (for quick testing) and Android. I was inspired by the computer game "Perimeter", but I'm not making a clone.

<div>
<img src="https://github.com/arnemvend/arnemvend/blob/main/images/01_AC.jpg" height="250"/>
<img src="https://github.com/arnemvend/arnemvend/blob/main/images/02_AC.jpg" height="250"/>
</div>

Now the game implements the construction of an energy network, the logic of the player's camera, the behavior model of units and the destruction of towers.

---
UPD
---
List of updates:
- All meshes have been changed. The game style has been defined. Not the final version.
- All effects have been changed. The game style has been defined. Not the final version.
- The structure with arrays of units has been moved to the Actor and is located in the world. (Compilation problem after UE5.5.1 update, temporary solution).
- Now all the effects of all explosions are controlled by one Actor who stores all particle systems and is in the world. (Slow operation with multiple calls of Explosion Actors, a temporary solution).
---
<div>
<img src="https://github.com/arnemvend/arnemvend/blob/main/images/03_AC.jpg" height="350"/>
<img src="https://github.com/arnemvend/arnemvend/blob/main/images/04_AC.jpg" height="350"/>
</div>


---
UPD
---
List of updates:
- Added weapons and their construction logic. Towers can now defend themselves.
- Added energy accumulation and spending logic.
- Core stats of units and weapons have been moved to GameInstance.
- Added automatic repair.
- Fixed shield mechanics.
- Fixed construction logic.
- Adjusted network search logic. Tower pointers are now stored in arrays within GameMode.

---
<div>
<img src="https://github.com/arnemvend/arnemvend/blob/main/images/05_AC.jpg" height="350"/>
<img src="https://github.com/arnemvend/arnemvend/blob/main/images/06_AC.jpg" height="350"/>
</div>



---
UPD
---
List of updates:
- Created a prototype of the main menu.
- Added the ability to exit the game.

---
<div>
<img src="https://github.com/arnemvend/arnemvend/blob/main/images/07_AC.jpg" height="350"/>
<img src="https://github.com/arnemvend/arnemvend/blob/main/images/08_AC.jpg" height="350"/>
</div>



---
UPD
---
List of updates:
- Settings menu added (interface only, features are currently disabled).