#include <unordered_map>
#include <list>
#include <queue>
#include <unordered_set>
#include <stack>
#include <limits>
#include <iostream>
#include <Windows.h>
#include <algorithm>

template<typename Vertex, typename Distance = double>
class Graph
{
public:
	struct Edge
	{
		Vertex from, to;
		Distance distance;
	};
	std::unordered_map<Vertex, std::list<Edge>> _array;

	void print()const
	{
		std::cout << "\nGraph:\n";
		for (auto const& pair : _array)
		{
			std::cout << pair.first << " - ";
			for (auto edge : pair.second)
			{
				std::cout << " { " << edge.from << "," << edge.to << "," << edge.distance << " }  , ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}

	bool has_vertex(const Vertex& v) const
	{
		if (_array.find(v) != _array.end())
			return true;
		return false;
	}

	void add_vertex(const Vertex& v)
	{
		if (!has_vertex(v))
			_array.insert({ v, std::list<Edge>() });
	}

	bool remove_vertex(const Vertex& v)
	{
		if (_array.erase(v) > 0)
		{
			std::vector<Vertex> vs = vertices();
			for (auto i : vs)
				remove_edge(i, v);
			return true;
		}

		return false;
	}

	std::vector<Vertex> vertices() const
	{
		std::vector<Vertex> result;
		for (auto const& pair : _array)
		{
			result.push_back(pair.first);
		}
		return result;
	}

	//проверка-добавление-удаление ребер
	void add_edge(const Vertex& from, const Vertex& to, const Distance& d)
	{
		if (!has_vertex(from) || !has_vertex(to))
			throw std::invalid_argument("Вершина не найдена");
		auto tmp = _array.find(from);
		tmp->second.push_back({ from,to,d });
	}
	
	bool remove_edge(const Vertex& from, const Vertex& to)
	{
		if (!has_edge(from, to))
			return false;
		auto tmp = _array.find(from);
		auto Iter = tmp->second.begin();
		for (auto const& edge : tmp->second)
		{
			if (edge.to == to)
				tmp->second.erase(Iter);
			Iter++;
		}
		return true;
	}
	
	bool remove_edge(const Edge& e) //c учетом расстояния
	{
		if (!has_edge(e))
			return false;
		auto tmp = _array.find(e.from);
		auto Iter = tmp->second.begin();
		while (Iter != tmp->second.end())
		{
			if (Iter->to == e.to && Iter->distance == e.distance)
			{
				tmp->second.erase(Iter);
				return true;
			}
			Iter++;
		}
		return false;
	}

	bool has_edge(const Vertex& from, const Vertex& to) const
	{
		if (!has_vertex(from))
			return false;
		auto tmp = _array.find(from);
		for (auto const& edge : tmp->second)
		{
			if (edge.to == to)
				return true;
		}
		return false;
	}

	bool has_edge(const Edge& e) const //c учетом расстояния в Edge
	{
		if (has_edge(e.from, e.to))
		{
			auto tmp = _array.find(e.from);
			for (auto const& edge : tmp->second)
			{
				if (edge.to == e.to)
				{
					if (edge.distance == e.distance)
						return true;
				}
			}
		}
		return false;
	}

	//получение всех ребер, выходящих из вершины
	std::vector<Edge> edges(const Vertex& vertex) const
	{
		if (!has_vertex(vertex))
			throw std::invalid_argument("Вершина не найдена");
		std::vector<Edge> result;
		auto tmp = _array.find(vertex);
		for (auto const& edge : tmp->second)
		{
			result.push_back(edge);
		}
		return result;
	}

	size_t order() const //порядок
	{
		return _array.size();
	}

	size_t degree() const //степень
	{
		std::vector<size_t> result;
		for (auto const& pair : _array)
		{
			result.push_back(pair.second.size());
		}
		size_t max = *std::max_element(result.begin(), result.end());
		return max;
	}

	//поиск кратчайшего пути Дейкстры
	std::vector<Edge> shortest_path(const Vertex& from, const Vertex& to) const
	{
		if (!has_vertex(from) || !has_vertex(to))
			throw std::invalid_argument("Вершина не найдена");
		std::vector<Edge> result;
		std::unordered_map < Vertex, std::pair<Vertex, Distance>> tmp; //пара {пред, расстояние до from}
		for (auto const& pair : _array)
		{
			tmp[pair.first].second = (std::numeric_limits<Distance>::max)();
		}
		tmp[from].second = 0;

		std::unordered_set<Vertex> vertices;
		vertices.insert(from);
		while (!vertices.empty())
		{
			auto tmp_pair = _array.find(*vertices.begin());
			for (auto edge : tmp_pair->second)
			{
				Distance d = tmp[*vertices.begin()].second + edge.distance;
				if (d < tmp[edge.to].second)
				{
					vertices.insert(edge.to);
					tmp[edge.to].second = d;
					tmp[edge.to].first = edge.from;
				}
			}
			vertices.erase(*vertices.begin());
		}

		if (tmp[to].second < (std::numeric_limits<Distance>::max)())
		{
			Vertex v = to;
			while (v != from)
			{
				Vertex prev = tmp[v].first;
				result.push_back({ prev,v, tmp[v].second - tmp[prev].second });
				v = prev;
			}
			std::reverse(result.begin(), result.end());
			return result;
		}
		else
			throw std::invalid_argument("Такого пути нет");
	}
	
	//обход в глубину
	std::vector<Vertex>  walk(const Vertex& start_vertex)const
	{
		if (!has_vertex(start_vertex))
			throw std::invalid_argument("Вершина не найдена");
		std::vector<Vertex> result;
		std::unordered_map<Vertex, bool> visited;
		for (auto const& pair : _array)
		{
			visited[pair.first] = false;
		}
		std::stack<Vertex> vertices;
		vertices.push(start_vertex);
		while (!vertices.empty())
		{
			Vertex v_tmp = vertices.top();
			if (!visited[v_tmp])
				result.push_back(v_tmp);
			visited[v_tmp] = true;
			vertices.pop();
			std::vector<Edge> edges_tmp = edges(v_tmp);
			for (const Edge& edge : edges_tmp)
			{
				if (!visited[edge.to])
				{
					vertices.push(edge.to);
				}
			}
		}
		return result;
	}
};

