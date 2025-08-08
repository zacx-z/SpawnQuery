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

## How to Use

Here are the typical steps to work with Spawn Query System:

 - Create your entry type deriving from `FSpawnEntryTableRowBase` and add your data fields to it. Create Data Tables with your entry data.
 - Create Spawn Query asset. Open the asset and edit the decision graph.
 - Call `QueryEntry()` function of the Spawn Query asset in your Blueprint class. It will return an entry.
 - Follow your own operations with the chosen entry data.

We can edit the Spawn Query graph to achieve various effects, including those mentioned in [Common Use Cases](http://nelasystem.com/lab/SpawnQuerySystem/common-use-cases/). For example, we are implementing a pity system guaranteeing a high-rarity drop in `X` drops:

 - Use a `Random Selector`, and set its random policy to Shuffled Sequence.
 - Put low-rarity items in one table, and high-rarity items in another. Create `Pool Sampler` nodes for them and link them to the `Random Selector` node.
 - Add `Weight Override` node to each sampler node. Set the weight `(X - 1) / 2` on the low-rarity branch, and the weight `1` on the high-rarity branch.

![Screenshot](http://nelasystem.com/lab/SpawnQuerySystem/images/Screenshot_Graph_PitySystem.png "Graph for Pity System")

For further information, check the [documentation](http://nelasystem.com/lab/SpawnQuerySystem).

## Future Work

 - Better debug information on the graph
 - Log panel to view the past results and statistics of a graph
 - Add slice feature to Pool Sampler node, allowing a portion of the pool table to be selected by the node.

