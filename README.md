# Pokemon Multiplayer Red

This is an attempt at porting Pokemon Red and Blue to PC with the addition of client-server online multiplayer. You and your friends will be able to team up and play the original game as it would be on your original Gameboy, with minimal changes to facilitate small group multiplayer. It's not meant to be like an MMORPG where you play a blend of different games with thousands of strangers you don't know. The project goal is analogous to becoming "Skyrim Together" instead of becoming "The Elder Scrolls Online".

## Project Archived - On Indefinite Hold

The original author of this project has put it on hold in order to focus on their own game development. A contributor has volunteered to update the project so it can easily be built using Visual Studio 2017 Community. The project is now configured to install and use unofficial NuGet packages for SFML.

## Building Pokemon Multiplayer Red

- **If you open the solution in Visual Studio 2019 you will be prompted to retarget projects. Do NOT upgrade the platform toolset. Doing so will break NuGet package compatibility.**
- **After you have opened the solution in Visual Studio for the first time, go to _Tools -> NuGet Package Manager -> Manage NuGet Packages for Solution..._ to open the package manager window. The window will have a prompt at the top for you to restore NuGet packages. Use the _Restore_ button to get and install the required packages.**

## Running Pokemon Multiplayer Red

The application currently runs using data dumps from [pokered](https://github.com/pret/pokered) and [ClassicMap](https://hax.iimarckus.org/post/43512/). The dumped resources aren't in this repository to avoid copyright issues. The author or contributor will try to find some way to distribute the data or make it in some way obtainable.

In the event that you do have the data available, you can place the data at _\<projectpmr\>\pmr\resources_ to have it be found when you run the application with debugging.
