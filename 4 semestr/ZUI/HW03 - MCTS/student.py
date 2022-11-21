import random, time
from collections import defaultdict
from typing import List, Dict, Callable

import numpy as np
from numpy import ndarray

import ox


class Bandit:
    def total_visits(self):
        raise NotImplementedError()

    def select(self):
        raise NotImplementedError()

    def update(self, value: float):
        raise NotImplementedError()

    def best_action(self):
        raise NotImplementedError()


def random_simulation(state: ox.Board):
    while not state.is_terminal():
        state.apply_action(random.choice(list(state.get_actions())))
    return state.get_rewards()

def node_depth(node: str):
  if node == "":
    return 0
  else:
    return int((len(node) - 1) / 3 + 1)


class UcbBandit(Bandit):
    def __init__(self, state: ox.Board, c_uct):
        self.c_uct = c_uct
        self.actions = list(state.get_actions())
        self.qs = np.zeros(len(self.actions))
        self.visits = np.zeros(len(self.actions))
        self.last_idx = None

    def total_visits(self) -> ndarray:
        return np.sum(self.visits)

    def select(self):
        total_visits = self.total_visits()
        if 0 not in self.visits:
            ucb_vals = (self.qs + self.c_uct * np.sqrt(np.log(total_visits) / self.visits))
        else:
            ucb_vals = self.qs

        self.last_idx = np.argmax(ucb_vals)
        return self.actions[self.last_idx]

    def update(self, value: float):
        self.visits[self.last_idx] += 1
        self.qs[self.last_idx] += (value - self.qs[self.last_idx]) / self.visits[self.last_idx]

    def best_action(self):
        return self.actions[np.argmax(self.qs)]



class MCTS:
    def __init__(self, bandit_factory: Callable[[ox.Board], Bandit],
                       simulate: Callable[[ox.Board], List[float]]):
        self.node_table: Dict[str, Bandit] = dict()
        self.bandit_factory = bandit_factory
        self.simulate = simulate

    def has_node(self, state: ox.Board) -> bool:
        return state in self.node_table

    def get_node(self, state: ox.Board) -> Bandit:
        return self.node_table[state]

    def make_node(self, state: ox.Board):
        self.node_table[state] = self.bandit_factory(state)

    def deepest_node(self) -> int:
        return max(node_depth(node) for node in self.node_table.keys())

    def node_stats(self) -> Dict[int, int]:
      # Count the number of visited nodes at each depth.
      nodes = sorted(self.node_table.keys())
      node_stats = defaultdict(int)
      for node, next_node in zip(nodes, nodes[1:]):
        node_stats[node_depth(node)] += 1
      return node_stats


    def best_action(self, state: ox.Board):
        return self.get_node(state).best_action()

    def step(self, state: ox.Board):

      if not self.has_node(state):
          self.make_node(state)

      trace, last_state = self.select(state.clone())

      # Compute values, either by asking the game, or by making a simulation.
      if last_state.is_terminal():
          values = last_state.get_rewards()
      else:
          self.expand(last_state)
          values = self.simulate(last_state.clone())

      # Backpropagate the values to make bandit updates along the trace,
      # except for the last node on the trace -- the leaf or the terminal.
      self.backpropagate(trace, values)

    def select(self, state: ox.Board):
        trace = []
        while self.has_node(state):
            trace.append(state.clone())
            if state.is_terminal():
                break
            action = self.get_node(state).select()
            if action in state.get_actions():
                state.apply_action(action)

        last_state = state.clone()
        return trace, last_state

    def expand(self, state: ox.Board) -> ox.Board:
        self.make_node(state)
        return state

    def backpropagate(self, trace, values: List[float]) -> None:
        for state in trace:
            self.get_node(state).update(values[state.current_player()])



class MCTSBot:
    def __init__(self, play_as: int, time_limit: float):
        self.play_as = play_as
        self.time_limit = time_limit * 0.9
        self.mcts = MCTS(lambda state: UcbBandit(state, 2), random_simulation)

    def play_action(self, board):
        start_time = time.time()

        while (time.time() - start_time) < self.time_limit:
            self.mcts.step(board)


        if self.mcts.best_action(board) in list(board.get_actions()):
            return self.mcts.best_action(board)
        else:
            return random.choice(list(board.get_actions()))


if __name__ == '__main__':
    board = ox.Board(8)  # 8x8
    bots = [MCTSBot(0, 0.1), MCTSBot(1, 0.1)]

    # try your bot against itself
    while not board.is_terminal():
        current_player = board.current_player()
        current_player_mark = ox.MARKS_AS_CHAR[ ox.PLAYER_TO_MARK[current_player] ]

        current_bot = bots[current_player]
        a = current_bot.play_action(board)
        board.apply_action(a)

        print(f"{current_player_mark}: {a} -> \n{board}\n")