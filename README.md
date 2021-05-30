# chomps
An AI designed to learn the winning strategy of the game chomps. It trains agaisnt a competing AI on each board size to learn all winning and losing patterns.
The trainng works by exhaustion, with the AI completing it's training once the competing AI marks all possible moves as losing ones.

The training process is simple: The two AIs play consecutive games, and memorize strategies that lead to a defeat. Once the AI recognizes that they can't win with a certain move, it learns to not make it again.
