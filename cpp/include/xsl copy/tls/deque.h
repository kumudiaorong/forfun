#ifndef TLS_DEQUE
#define TLS_DEQUE
#include"container.h"
#include"arg.h"
#include"wrapper.h"
#include"array.h"
namespace tls {
	//tls deque mini len
#define TLS_DML 10
	template<typename Val>
	class deque :public container {//10
		using Wpr = wrapper<Val>;

		Wpr** Map;//map
		uint_t Head, Tail, MapSize;
	public:
		using _Val = Val;
		constexpr void expand() {
			Wpr** newMap = new Wpr * [MapSize * 2];
			for (uint_t i = (Head / TLS_DML + 1) % MapSize; i != Tail / TLS_DML; ++i %= MapSize)
				newMap[i] = Map[i];
			Tail = Head + (Tail - Head + MapSize * TLS_DML) % (MapSize * TLS_DML);
			for (uint_t i = 0; i < TLS_DML; ++i)
				Map[Tail / 10 + i] = nullptr;
			delete[] Map;
			Map = newMap;
			MapSize *= 2;
		}
		template<typename...Aoo>
		constexpr void push_back_helper(Aoo&&...argsOrObj) {
			uint_t newTail = (Tail + 1) % (MapSize * TLS_DML);
			if (Tail % TLS_DML == 0)
			{
				if ((Head - Tail + MapSize * TLS_DML) % (MapSize * TLS_DML) < TLS_DML)expand();
				if (Map[newTail / TLS_DML] == nullptr)
					Map[newTail / TLS_DML] = new char[sizeof(Wpr) * TLS_DML];
			}
			new (Map[newTail / TLS_DML] + newTail % TLS_DML)Val(forward<Aoo>(argsOrObj)...);
			Tail = newTail;
		}
		template<typename...Aoo>
		constexpr void push_front_helper(Aoo&&...argsOrObj)
		{
			uint_t newHead = (Head - 1 + MapSize * TLS_DML) % (MapSize * TLS_DML);
			if (Head % TLS_DML == 0)
			{
				if ((Head - Tail + MapSize * TLS_DML) % (MapSize * TLS_DML) < TLS_DML)expand();
				if (Map[newHead / TLS_DML] == nullptr)
					Map[newHead / TLS_DML] = new char[sizeof(Wpr) * TLS_DML];
			}
			new (Map[newHead / TLS_DML] + newHead % TLS_DML)Val(forward<Aoo>(argsOrObj)...);
			Head = newHead;
		}
	public:
		constexpr deque()
			:Map(new Wpr*), Head(0), Tail(1), MapSize(1) {
			TEST(deque());
			Map[0] = (Wpr*)new char[sizeof(Wpr) * TLS_DML];
		};
		constexpr deque(deque&& Another)
			:Map(Another.Map), Head(Another.Head), Tail(Another.Tail), MapSize(Another.MapSize) {
			TEST(deque(deque && Another));
			Another.Map = nullptr;
		}
		constexpr deque(const deque& Another)
			: Map(new Wpr* [Another.MapSize]), Head(Another.Head), Tail(Another.Tail), MapSize(Another.MapSize)
		{
			TEST(deque(const deque & Another));
			for (uint_t i; i < MapSize; ++i)
				Map[i] = new char[sizeof(Wpr) * TLS_DML];
			for (uint_t i = (Head + 1) % (MapSize * TLS_DML); i != Tail; ++i %= (MapSize * TLS_DML))
				new (Map[i / TLS_DML] + i % TLS_DML)Val(Another.Map[i / TLS_DML][i % TLS_DML]);
		}
		constexpr Val& at(uint_t pos) {
			pos = (pos + Head) % (MapSize * TLS_DML);
			return Map[pos / TLS_DML][pos % TLS_DML];
		}
		constexpr Val& back() {
			TEST(Val & back());
			uint_t tmp = (Tail - 1 + MapSize * TLS_DML) % (MapSize * TLS_DML);
			return Map[tmp / TLS_DML][tmp % TLS_DML];
		}
		template<typename...Args>
		constexpr void emplace_back(Args&&...args) {
			TEST(void emplace_back(Args&&...args));
			push_back_helper(forward<Args>(args)...);
		}
		template<typename...Args>
		constexpr void emplace_front(Args&&...args) {
			TEST(void emplace_front(Args&&...args));
			push_front_helper(forward<Args>(args)...);
		}
		constexpr bool empty() {
			TEST(bool empty());
			return !size();
		}
		constexpr Val& front() {
			TEST(Val & front());
			uint_t tmp = (Head + 1) % (MapSize * TLS_DML);
			return Map[tmp / TLS_DML][tmp % TLS_DML];
		}
		constexpr void pop_back() {
			TEST(void pop_back());
			Tail = (Tail - 1 + MapSize * TLS_DML) % (MapSize * TLS_DML);
			Map[Tail / TLS_DML][Tail % TLS_DML].~Wpr();
		}
		constexpr void pop_front()noexcept
		{
			TEST(void pop_front());
			Head = (Head + 1) % (MapSize * TLS_DML);
			Map[Head / TLS_DML][Head / TLS_DML].~Wpr();
		}
		//args for one obj or objs
		constexpr void push_back(Val&& obj)
		{
			TEST(void push_back(Val && obj));
			push_back_helper(static_cast<Val&&>(obj));
		}
		//args for one obj or objs
		constexpr void push_back(const Val& obj)
		{
			TEST(void push_back(const Val & obj));
			push_back_helper(obj);
		}
		//args for one obj or objs
		constexpr void push_front(Val&& obj)
		{
			TEST(void push_front(Val && obj));
			push_front_helper(static_cast<Val&&>(obj));
		}
		//args for one obj or objs
		constexpr void push_front(const Val& obj)
		{
			TEST(void push_front(const Val & obj));
			push_front_helper(obj);
		}
		constexpr uint_t size() {
			TEST(uint_t size());
			return (Tail - Head + MapSize * TLS_DML) % (MapSize * TLS_DML);
		}
		constexpr ~deque() {
			TEST(~deque());
			if (Map) {
				for (uint_t i = (Head + 1) % (MapSize * TLS_DML); i != Tail; ++i %= (MapSize * TLS_DML))
					Map[i / TLS_DML][i % TLS_DML].~Wpr();
				for (uint_t i = Head / TLS_DML; i != Tail / TLS_DML; ++i %= MapSize)
					delete[](char*)Map[i];
				delete[](char*)Map[Tail / TLS_DML];
				delete[]Map;
			}
		}
	};
}

#endif//TLS_DEQUE