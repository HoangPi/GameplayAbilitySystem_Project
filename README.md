# GASP6

Developed with Unreal Engine 5

# Brief
Implement <code>Gameplay Ability System</code> provided by <code>Unreal Engine</code> in <code>native C++ code</code> to create dynamic, highly customizable <code>Gameplay Abilities</code> that interact with <code>Gameplay Effect</code>, <code>Attribute Set</code> and <code>Gameplay Tag</code>. The project also demonstrates extended use of <code>UPROPERTY</code> and <code>UFUNCTION</code> to bridge the gap between <code>native code</code> and Unreal Engine's <code>Blueprint</code>; and ease the communication between <code>System Developer</code> and <code>Asset Artist</code>.

# Avaiable Features
## Sprint Functionality
While user is holding <code>Sprint Input</code> (bound to <code>Shift</code>), the player character's max walk speed will increase and trigger a <code>gameplay effect</code> to drain <code>stamina attribute</code>. When <code>stamina attribute</code> is depleted or when user release the <code>Sprint Input</code>, the ability will stop and after a brief moment, another <code>gameplay effect</code> will trigger and steadyly regenerate player's stamina to full.
## Lock On Target
When user starts <code>Lock On Input</code> (bound to <code>Middle Mouse Button</code> or <code>Q</code>), a the Engine will perform a trace that scan the area before player's viewpoint and get the first <code>Actor</code> with <code>Enemy collision channel</code>. If the trace returns a valid <code>Actor</code>, <code>player's controller</code> will constantly rotate toward that <code>Actor</code>, this effect will persist untill the line of sight between player and that <code>Actor</code> is severed or if player start another <code>Lock On Input</code>. While player is locking on, if user triggers <code>Look Input</code> (bound to <code>Mouse Movement</code>) that pass a certain intensitive threshold, the Engine will start another trace either to the left or the right, depends on user's input to trace a different <code>Actor</code>, effectively change the <code>Lock On Target</code>.
## Guard Functionality
- User can press the keyboard button **E** or **N** to spawn a <code>Projectile</code> that will constantly move toward player and deal damage when it hits player.
- If the <code>Projectile</code> hit player while player is <code>Guarding</code>, the damage will be transfered to <code>Stamina Attribute</code> instead of <code>Health Attribute</code>.
