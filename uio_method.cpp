#include "uio_method.h"
#include <queue>
#include <map>
#include "util.h"
#include "cover.h"

bool search(set<node> nodes, const string& node)
{
	for (const auto& n : nodes)
	{
		if (n.current == node)
		{
			return true;
		}
	}
	return false;
}

void add_solution(state st, list<state>* sol)
{
	if (sol->empty())
	{
		sol->push_back(st);
		return;
	}
	for (auto state : *sol)
	{
		if (st.nodes.begin()->parent == state.nodes.begin()->parent)
		{
			return;
		}
	}
	sol->push_back(st);
}

bool is_in_states(const state& state1, list<state> *states)
{
	for (const auto& state2 : *states)
	{
		if (state1.nodes == state2.nodes) return true;
	}
	return false;
}

bool is_all_node_same(state state)
{
	if (state.nodes.size() > 1)
	{
		const string current = state.nodes.begin()->current;
		for (const auto& node : state.nodes)
		{
			if (!(current == node.current)) return false;
		}
	}
	else return false;
	return true;
}
bool check(state state1, list<state> *states)
{
	if (state1.nodes.size() > 1)
	{
		if (is_all_node_same(state1)) return false;
		if (is_in_states(state1, states)) return false;
	}
	else return false;
	return true;
}

list<state> find_children(state parent, const list<transition> transitions, list<state> *states, list<state> *sol)
{
	list<state> children;
	//split
	map<string, list<transition>> input_outputs;
	for (const auto& transition : transitions)
	{
		if (search(parent.nodes, transition.state))
		{
			input_outputs[transition.input + ":" + transition.output].push_back(transition);
		}
	}

	for (const auto& pair : input_outputs)
	{
		state st;
		st.input = parent.input + " " + pair.first;

		for (const auto& node1 : parent.nodes)
		{
			for (const auto& transition : input_outputs[pair.first])
			{
				if (node1.current == transition.state)
				{
					node nd;
					nd.parent = node1.parent;
					nd.current = transition.next_state;
					st.nodes.insert(nd);
					break;
				}
			}
		}

		if (check(st, states))
		{
			children.push_back(st);
		}

		if (st.nodes.size() == 1)
		{
			add_solution(st, sol);
		}
	}
	for (const auto& child : children)
	{
		states->push_back(child);
	}
	return children;
}

void build(const state& root, const list<transition>& transitions, list<state> *states, list<state> *sol)
{
	queue<state> queue;
	queue.push(root);
	while (!queue.empty())
	{
		const auto state = queue.front();
		queue.pop();
		auto children = find_children(state, transitions,states,sol);

		for (const auto& child : children)
		{
			queue.push(child);
			//child.to_string();
		}
	}
}

set<list<string>> get_inputs(list<state>* sol)
{
	set<list<string>> inputs;
	char* context = nullptr;
	for (auto state : *sol)
	{
		const auto ca = &state.input[0];
		auto token = strtok_s(ca, ": ", &context);
		list<string> input;
		while (token != nullptr)
		{
			input.emplace_back(token);
			strtok_s(nullptr, ": ", &context);
			token = strtok_s(nullptr, ": ", &context);
		}
		inputs.insert(input);
	}
	return inputs;
}

set<string> get_state_names(list<transition> transitions)
{
	set<string> state_names;
	for (const auto& transition : transitions)
	{
		state_names.insert(transition.state);
		state_names.insert(transition.next_state);
	}
	return state_names;
}


state get_state(set<string> state_names)
{
	state state;
	for (const auto& node1 : state_names)
	{
		node nd;
		nd.parent = node1;
		nd.current = node1;
		state.nodes.insert(nd);
	}
	return state;
}


set<list<string>> uio_method::run(const pair<string, list<transition>> transitions)
{
	const auto sol = new list<state>;
	auto states = new list<state>;
	const auto state_names = get_state_names(transitions.second);
	const auto state = get_state(state_names);
	states->push_back(state);
	build(state, transitions.second,states,sol);
	const auto inputs = get_inputs(sol);
	const auto formatted_transitions = util::get_formatted_transitions(transitions.second);
	const auto p = cover::get_transition_cover_set(formatted_transitions, transitions.first);
	auto t = cover::get_t(p, inputs);
	return t;
}
