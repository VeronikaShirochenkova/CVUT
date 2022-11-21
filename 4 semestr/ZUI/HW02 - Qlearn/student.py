from blockworld import BlockWorldEnv
import random, numpy as np
import operator

class QLearning():
	# don't modify the methods' signatures!
	def __init__(self, env: BlockWorldEnv):
		self.env = env
		self.state_dict = {}

	def policy(self, state, epsilon=0.1):
		if np.random.rand() > epsilon:
		# выбрать по ключу стейт самое big Q, return key
			a = max(self.state_dict[state].items(), key=operator.itemgetter(1))[0]
			print(a)
		else:
			a = random.choice(state[0].get_actions())
		return a

	def add_state(self, state):
		self.state_dict[state] = {}
		for act in state[0].get_actions():
			self.state_dict[state][act] = 0

	def train(self):
		# Use BlockWorldEnv to simulate the environment with reset() and step() methods.
		# s = self.env.reset()
		# s_, r, done = self.env.step(a)
		alpha = 0.9
		gamma = 0.5


		while True:
			state = self.env.reset()
			done = False
			if state not in self.state_dict:
				self.add_state(state)


			while not done:
				# Choose action
				action = self.policy(state, epsilon=0.1)
				# Do the action
				next_state, reward, done = self.env.step(action)
				if next_state not in self.state_dict:
					self.add_state(next_state)
				# Update q_values
				# maxQ() in dict key=next_state find max value by key=action
				k =  max(self.state_dict[next_state].items(), key=operator.itemgetter(1))[0]
				self.state_dict[state][action] = self.state_dict[state][action] + alpha * (reward + gamma * self.state_dict[next_state][k] - self.state_dict[state][action])
				# Update state
				state = next_state


	def act(self, s):
		if s in self.state_dict:
			action = max(self.state_dict[s].items(), key=operator.itemgetter(1))[0]
		else:
			action = random.choice( s[0].get_actions() )
		return action

if __name__ == '__main__':
	# Here you can test your algorithm. Stick with N <= 4
	N = 4

	env = BlockWorldEnv(N)
	qlearning = QLearning(env)

	# Train
	qlearning.train()

	# Evaluate
	test_env = BlockWorldEnv(N)

	test_problems = 10
	solved = 0
	avg_steps = []

	for test_id in range(test_problems):
		s = test_env.reset()
		done = False

		print(f"\nProblem {test_id}:")
		print(f"{s[0]} -> {s[1]}")

		for step in range(50): 	# max 50 steps per problem
			a = qlearning.act(s)
			s_, r, done = test_env.step(a)

			print(f"{a}: {s[0]}")

			s = s_

			if done:
				solved += 1
				avg_steps.append(step + 1)
				break

	avg_steps = sum(avg_steps) / len(avg_steps)
	print(f"Solved {solved}/{test_problems} problems, with average number of steps {avg_steps}.")