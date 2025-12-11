# Entities

Entity generally means a Node that "exists" in the game world. If the player can see an object, it's an entity.

Usually an entity should clean itself up based on a timer or position.

```cpp
void PropBrk::_process(double delta) {
	if (Game::is_out_of_bounds(this)) {
		queue_free();
		return;
	}
}
```