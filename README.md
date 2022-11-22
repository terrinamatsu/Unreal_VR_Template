# Pipeline And Technical Direction Document
## Pipeline Option
Jacob Worgan, s5107963
<br>
Source code for this project is located in the ./Source/ folder.
___
## Breakdown of Project Requirements
<p> For this project, I decided to create an updated VR template for use in Unreal Engine 4.27. I have been involved in several VR projects in the past using Unreal Engine, and found the provided default VR template to be inadequate as it is simpler than the related project templates for a device interface that necessitates more initial development for player comfort and useability. This would also allow me to create the template in C++ rather than blueprints, important as the original template is made in blueprints. Because C++ code cade be used in blueprints, but blueprints cannot be used in C++ code, creating the template in this way means that the future user has the option to use either blueprints or C++ to build off the template, with extra care taken during the development of the template to exposing the key parts of the code to blueprints level. </p>
<br>
<p>
The requirements for this project include:
</p>

- A fully featured VR Player Pawn with:
    - Movement
    - Turning
    - Gripping Objects
    - In editor Cube component to give reference to standard VR space size when building levels.
    - On/Off options for each of the player controls. 
    - Options to setup for various Head Mounted Display (HMD) types (e.g. Oculus, HTC Vive, etc.)

- Gippable Object Component with:
    - Options for snapping to player hand
    - Virtual functions that are called on grip start, hold, release, and on action start, hold, and release to allow the user to easily build interactable objects.

- 'Out of Bounds' Collision Box Component with:
    - Camera fade when entering with smooth transition to let the player know when they are moving out of bounds.

- Navigation Procedural Mesh Component that:
    - Takes input navigation data and renders a mesh, which the VR Pawn hides and shows when teleporting to let the player know where they are able to walk.
___
## Installation Guide
<p>
This project should have a relatively easy install proces. Simply clone the repository into a blank folder and open the ureal project file in Unreal Engine version 4.27 (Unreal provides the option to open in other engine versions both earlier and more recent, however this is untested).
<br>
At this point, the project should run as a standard Unreal project. To start coding, right click the Unreal Project file in the main folder and click "Generate Visual Studio project files". THis will create a .slln file which can be opened in a compatible version of visual studio. 
</P>

___
## Usage Guide
<p>
This project has been designed to work without needing to interact with any C++ code directly, and so guidance will primarily be given for using the coded elements in Blueprints.  
</p>

### 1. The Defaults
On opening the project, the editor will load into the 'TestLevel' map, featuring elements that demonstrate what the project can do. This includes blueprint objects that inherit and include from the base C++ classes of 'OutOfBoundsBox', 'GripObject', and the 'VRPlayerPawn' (these can also be found in the contents folder under /Blueprint/). 
___
### 2. Tests
A range of tests have been written to aid in the development of this project, using Unreal's Automation Testing system. These can be run by selecting /Window/Testing/Test Automation/ on the top bar in the Unreal Editor. Select the application on the left, then the automation tab on the right section of the screen. You can then select and run tests. The tests I have written for this project are under 'Components', 'GrippableComponent', 'VRFunctionLibarary', and 'VRPlayerPawn'. Try to only run a couple of tests at a time, as running too many tests may cause the editor to not respond. Currently, the 'Components' tests fail as this is the next stage of active development of this project, and there are also warnings on tests including the VRPlayerPawn for the same reason. 
___
### 3. The VR Player Pawn
The VR Player Pawn is the main part of the project, as it interfaces between the HMD and the world. Clicking on the BP_VRPlayerPawn (or opening the BP_VRPlayerPawn blueprint, or creating a new inherited class from the C++ VRPlayerPawn) and looking at the default values you can see the primary options for player controls for this VR system. 

#### 3.1 VR
The first subfolder in the details pannel is titled VR. Here are the references to various objects the VR Player Pawn interacts with, including Gripped Objects and the Nav Mesh. These are automatically assigned when the game is running, although the Nav Mesh does sometimes need to be manually selected. 

#### 3.2 Device Setup & HMD
The second subfolder is Device Setup, where options relating to the Head Mounted Display are. <br>Device type allows you to select which device you are using, including an automatic detection option. This visually changes the hand mesh to that of the selected HMD's controller, if the 'Use Hand Meshes' option is unchecked. 
<br>Run in VR allows you to disable the game running in VR mode on the HMD is unchecked. 
<br>Device Setup in the HMD subfolder includes a button that runs a function to do the related setup after choosing settings in the Device Setup subfolder. This function runs automatically on Begin Play, but the button allows you to make the change in editor without having to go into game. 

#### 3.3 Speed
The fourth subfolder is Speed, and allows you to select the Player movement (translation) and rotation speeds, movements that are controlled by the control sticks in game. 

#### 3.4 Enables
The fifth subfolder is Enables, and includes many of the options that changes how the player moves about and interacts with the space.  
<br>Movement Type allows you to select which teleportation style you wish to use (currently all use Direction Teleportation) 
<br>Turn Type allows you to select whether you with to smoothly rotate with the right stick, snap at 35 or 45 degree intervals, or not allow the player to rotate.  
<br>Teleport Style selects whether to use straight or arc teleportation. Using straight, you directly point the controller where you wish to go, and Arc simulates a gravity controlled arc to where you wish to go, which offers an easier, more natural style of teleportation.  
<br>Max Teleport Distance is only used when Teleport Style is set to straight, and sets the limit of how far you can teleport.  
<br>Teleport Arc Radius, Strength, and Sim Time are only used when Teleport Style is set to Arc, and sets the radius of the  cast sphere, the strength of the force it is intially thrown with, and the time this simulation can last before it fails respectively. 
<br>Can Teleport Across Gaps is a bolean to choose whether or not you want the player to be able to teleport across gaps in the Navigation Mesh or not.   
<br>Nav Point Visualiser When 'Can Teleport Across Gaps' is false, this is the static mesh of the points along the generated path to the destination point, testing for a future feature where the player is simulated to walk between these points.  
<br>The remaining enables turn on and off each of the player controls, useful for things like cutscenes or tutorial sections. 
___

### 4 Grippable Component
The Grippable component can be attached to any object to allow it to be gripped by the player pawn. It has one exposed parameter, Grip Type, which determines whether the object stays In Place, but is attached to the gripping hand and so moves relative to it, Snaps to the hand location and also attaches, or has No Attachment, and so does not attach or move, useful for objects like switches that the player can interact with but cannot move around. 
#### 4.1 Exposed Delegates
In blueprints containing the grippable component, you can add events (found in the details pannel of the grippable component) where you can add functionality on gripping, while gripping, and when releasing the object, or when pressing the second 'action' button on the controller where an object is being gripped for similar on action, during action, and end action functions. You can see an example of this in the BP_PickupTest blueprint. 

___

### 5 Out of Bounds Box

The out of bounds box component is a box with two parameters, fade colour and fade audio option. When the player's head component enters the box, a sphere trace is carried out to see how far into the box the component is, and then fades the screen gradually to the fade colour. This component is designed for out of bounds areas, as it is difficult to prevent the player from moving out of the playable area in VR, so with this you can instead fade the screen to make sure the player doesn't see anything they shouldn't be able to. 

___

### 6 Navigation Procedural Mesh Component

This is a component that allows the navigation mesh to be visible in game, using very finicky and scarcely documented functions of the Navigation Mesh found only in C++ and not exposed to blueprints, that allow the retrieval of the verts of the nav mesh. These are then fed into a Procedural Mesh that is rendered and updated. By default there is a Navigation Procedural Mesh attached to the VR Player Pawn that becomes visible when pressing the teleport button to let the player know where they are able to teleport to in the map. 

___

### 7 Snap Box Component

As mentioned this is unfinished, but will allow the player to snap object to the component, useful for keeping track of objects throughout the play space. 