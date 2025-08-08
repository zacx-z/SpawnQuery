# Spawn Query System

Spawn Query System is a graph-based tool that enables designers to define sophisticated randomization for spawning various things, such as loot, enemies, quest rewards, etc.

## Features

 - Configure spawnable entries in a custom table (pool)
 - Use a decision tree to decide the pool to spawn an item from, which adapts to in-game changes via Blackboard
 - Dynamically enabled/disabled subtrees in a decision tree
 - Allow in-game variables to influence probabilities of entries via Blackboard
 - Provide Shuffled Sequence randomization algorithm besides the traditional one.
 - Multiple contexts management and debugging
 - Allow customized sampler nodes with Blueprints

## Installation

Clone the repo into your Plugins directory:

`cd <your-project-root>/Plugins/ && git clone git@github.com:zacx-z/SpawnQuery.git`

Open the project. Open Plugins window via `Edit > Plugins`. Search for "Spawn Query" and enable the plugin. You may need to restart Unreal to make the plugin fully functional.

## How to Use.

Check the [documentation](http://nelasystem.com/lab/SpawnQuerySystem).

## Future Work

 - Better debug information on the graph
 - Log panel to view the past results of a graph
 - Add slice feature to Pool Sampler node, allowing a portion of the pool table to be selected by the node.

