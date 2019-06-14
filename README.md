# MeshGen
Mesh generation plugin for ue4


# Installing Unreal engine
- [linux quick start](https://docs.unrealengine.com/en-US/Platforms/Linux/BeginnerLinuxDeveloper/SettingUpAnUnrealWorkflow/index.html)
- [windows quick start](https://docs.unrealengine.com/en-US/GettingStarted/Installation/index.html)


# Adding the plugin
open up `{your project}.uproject` in a text editor, and add the following:

```
	"Plugins": [
		{
			"Name": "MeshGen",
			"Enabled": true
		}
	]
}
```

the whole uproject file should look something like this:

```
{
	"FileVersion": 3,
	"EngineAssociation": "4.22",
	"Category": "",
	"Description": "",
	"Modules": [
		{
			"Name": "{your project}",
			"Type": "Runtime",
			"LoadingPhase": "Default",
			"AdditionalDependencies": [
				"Engine"
			]
		}
	],
	"Plugins": [
		{
			"Name": "MeshGen",
			"Enabled": true
		}
	]
}
```


# using default generator class (SIMPLE)
1. In folder `{your project}/Config`, create a file called `MeshGen.ini`
2. Populate it with two arguments:
```
generator_type: SIMPLE
settings_file: {generator settings file name and location}
```
you can see & set other generator types in [GeneratorFactory](https://github.com/msb336/MeshGen/blob/master/Source/MeshGen/Public/GeneratorFactory.h)
If no settings file is provided, it will fill the parameters with default values. You can see how settings file is read in [GeneratorBase constructor](https://github.com/msb336/MeshGen/blob/master/Source/MeshGen/Private/GeneratorBase.cpp)

3. In Unreal Editor, click and drag MeshGenerator class object from content browser into scene. **Tip:** To see plugin content, click `View Options` at the bottom right corner of the Content Browser window and toggle `Show Plugin Content`
4. Click `Play` and the generator should start doing it's work!


# Creating your own generator class
[GeneratorBase](https://github.com/msb336/MeshGen/blob/master/Source/MeshGen/Public/GeneratorBase.h) is the Parent class for all procedural generator types. the only function that must be overriden is `loadTilesAsync(const TileList&)`. 

The way the MeshGenerator Works is through a series of steps:

1. Determine which "tile" the player pawn is occupying
2. Determine which tiles should be rendered in game, and which tiles should be loaded into memory
3. load and render relevant tiles, and unload / derender obsolete ones.
4. repeat 1-3


Custom mesh constructors must define the vertices, connectivity index, and texture of tiles as calls are made to the loader. [SimpleGenerator](https://github.com/msb336/MeshGen/blob/master/Source/MeshGen/Public/SimpleGenerator.h) uses perlin noise to create a 2d plane with texture relative to vertex height.


