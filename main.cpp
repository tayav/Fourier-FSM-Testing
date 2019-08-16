#include"util.h"
#include "w_method.h"
#include "f_method.h"
#include "cover.h"
#include "uio_method.h"

void print(string name , set<list<string>> const &test_suite)
{
    cout << "****************************"<<name<<"********************************" << endl << "[";
    for (auto const& list : test_suite){
        cout << ",{";
        for (auto const& i: list) {
            cout << i << ",";
        }
        cout << "}";
    }
    cout << "]" << endl;
}

void start(const char *file)
{
			const auto transitions = util::read(file);
			auto transitions_binary_form = util::get_transitions_binary_form(transitions);
			const auto formatted_transitions = util::get_formatted_transitions(transitions_binary_form.second);
			const auto inputs = util::get_inputs(transitions_binary_form.second);
			const auto states = util::get_states(transitions_binary_form.second);

			/**************************************************/
			const auto z = w_method::run(formatted_transitions, inputs, states);
			const auto p = cover::get_transition_cover_set(formatted_transitions, transitions_binary_form.first);
			auto w = cover::get_t(p, z);
			/*************************************************/
			auto uio = uio_method::run(transitions_binary_form);
			/*************************************************/
			const auto f = f_method::run(transitions_binary_form, 10, 10);
			/*************************************************/
			auto fw = cover::get_t(f, z);
			print("W-method",w);
			print("UIO-Method",uio);
			print("F-Method",f);
            print("Fw-Method",fw);


}
int main(int argc, char** argv)  {

	//start("test.kiss2");
    start(argv[1]);
	getchar();
    return 0;
}
