#pragma once
#include <variant>
#include <optional>



template <class Context, class ...States >
class FSM
{
	using StateVariant = std::variant<States...>;
	using OptionalStateVariant = std::optional<StateVariant>;
public:
	virtual ~FSM() = default;

	explicit FSM(StateVariant&& initialState, Context&& context);
	void update();
	Context& context();

private:
	StateVariant _curState;
	Context _context;
};


template<class Context, class ...States>
inline FSM<Context, States...>::FSM(StateVariant&& initialState, Context&& context) : _curState(std::move(initialState)), _context(std::move(context))
{

}

template<class Context, class ...States>
inline void FSM<Context, States...>::update()
{
	std::visit([&context = _context](auto& state) {
		state.update(context);
	}, _curState);

	auto newState = std::visit([&context = _context](auto& state) {
		return transition(state, context);
	}, _curState);

	if (newState) {
		_curState = std::move(newState.value());
	}
}

template<class Context, class ...States>
inline Context& FSM<Context, States...>::context()
{
	return _context;
}