#include "Graph.cpp"

/*3. Пусть дан некоторый связный граф, в котором узлы – это травмпункты.Предположим, в преддверии зимы вы хотите улучшить
покрытие города.Для этого вам необходимо понять, какой травмпункт находится дальше всего от своих прямых соседей
(т.е.средняя длина связанных с ним ребер максимальна).Напишите функцию, которая находит такой травмпункт.*/
template<typename Vertex, typename Distance = double>
Vertex fracture_clinic(const Graph<Vertex, Distance>& graph)
{
	if (graph.order() == 0)
		throw std::logic_error("Граф пуст");
	if (graph.order() < 3)
		throw std::logic_error("Травмпунктов слишком мало!");
	std::unordered_map<Vertex, std::pair<Distance, int>> distances;
	for (auto const& v : graph.vertices())
	{
		distances[v].first = 0;
		distances[v].second = 0;
	}
	for (auto const& v : graph.vertices())
	{
		for (auto const& edge : graph.edges(v))
		{
			distances[edge.to].first += edge.distance;
			distances[edge.to].second++;
			distances[edge.from].first += edge.distance;
			distances[edge.from].second++;
		}
	}
	Distance max = 0;
	for (auto& i : distances)
	{
		if (i.second.second != 0) // проверка на деление на 0, чтобы избежать ошибки
		{
			i.second.first = i.second.first / i.second.second;
			if (i.second.first > max)
				max = i.second.first;
		}
	}
	const double tolerance = 0.001; // определенная точность сравнения
	for (auto const& pair : distances)
	{
		if (std::abs(pair.second.first - max) < tolerance) // сравнение с определенной точностью
			return pair.first;
	}
}

int menu1()
{
	int type;
	std::cout << std::endl << "0\tСоздание графа с 0" << std::endl;
	std::cout << "1\tРабота с уже введенным графом" << std::endl;
	std::cin >> type;
	return type;
}

int menu2()
{
	int type;
	std::cout << std::endl << "1\tДобавить вершину" << std::endl;
	std::cout << "2\tУдалить вершину" << std::endl;
	std::cout << "3\tДобавить ребро" << std::endl;
	std::cout << "4\tУдалить ребро" << std::endl;
	std::cout << "5\tПолучить все ребра, выходящие из вершины" << std::endl;
	std::cout << "6\tПолучить порядок и степень графа" << std::endl;
	std::cout << "7\tОбход в грубину" << std::endl;
	std::cout << "8\tПоиск минимального пути" << std::endl;
	std::cout << "9\tЗадача(наиболее удаленный от соседей)" << std::endl;
	std::cout << "0\tВыход\n" << std::endl;
	std::cin >> type;
	return type;
}

template<typename Vertex, typename Distance = double>
void program(Graph<Vertex, Distance>& graph)
{
	while (true)
	{
		graph.print();
		int m = menu2();
		switch (m)
		{
			case 0:
			{
				return;
			}

			case 1:	//+вершину
			{
				Vertex vertex;
				std::cout << "Введите вершину - ";
				std::cin >> vertex;
				graph.add_vertex(vertex);
				system("cls");
			}
			break;

			case 2:	//-вершину
			{
				Vertex vertex;
				std::cout << "Введите вершину - ";
				std::cin >> vertex;
				graph.remove_vertex(vertex);
				system("cls");
			}
			break;

			case 3: //+ребро
			{
				Vertex v_start, v_end;
				Distance d;
				std::cout << "Введите начальную вершину - ";
				std::cin >> v_start;
				std::cout << "Введите конечную вершину - ";
				std::cin >> v_end;
				std::cout << "Введите расстояние - ";
				std::cin >> d;
				system("cls");
				try
				{
					graph.add_edge(v_start, v_end, d);
				}
				catch (std::exception& e)
				{
					std::cout << e.what() << std::endl;
				}
			}
			break;

			case 4: //-ребро (с учетом расстояния)
			{
				Vertex v_start, v_end;
				Distance d;
				std::cout << "Введите начальную вершину - ";
				std::cin >> v_start;
				std::cout << "Введите конечную вершину - ";
				std::cin >> v_end;
				std::cout << "Введите расстояние - ";
				std::cin >> d;
				graph.remove_edge({ v_start, v_end,d });
				system("cls");
			}
			break;

			case 5: //все ребра, выходящие из вершины
			{
				Vertex vertex;
				std::cout << "Введите вершину - ";
				std::cin >> vertex;
				system("cls");
				try
				{
					auto edges = graph.edges(vertex);
					std::cout << "Ребра из вершины " << vertex;
					for (auto edge : edges)
					{
						std::cout << " { " << edge.from << "," << edge.to << "," << edge.distance << " }  , ";
					}
				}
				catch (std::exception& e)
				{
					std::cout << e.what() << std::endl;
				}
			}
			break;

			case 6: //порядок и степень графа
			{
				system("cls");
				std::cout << "\nПорядок графа - " << graph.order() << "\nСтепень графа - " << graph.degree() << std::endl;
			}
			break;

			case 7: //обход в глубину
			{
				Vertex v_start;
				std::cout << "Введите начальную вершину - ";
				std::cin >> v_start;
				system("cls");
				try
				{
					std::vector<Vertex> vertices = graph.walk(v_start);
					std::cout << "\nОбход: " << std::endl;
					for (Vertex v : vertices)
					{
						std::cout << v << " ";
					}
					std::cout << std::endl;
				}
				catch (std::exception& e)
				{
					std::cout << e.what() << std::endl;
				}
			}
			break;

			case 8: //поиск минимального пути
			{
				Vertex v_start, v_end;
				std::cout << "Введите начальную вершину - ";
				std::cin >> v_start;
				std::cout << "Введите конечную вершину - ";
				std::cin >> v_end;
				system("cls");
				try
				{
					auto edges = graph.shortest_path(v_start, v_end);
					Distance d = 0;
					std::cout << "\nПуть:" << std::endl;
					for (auto edge : edges)
					{
						std::cout << " { " << edge.from << "," << edge.to << "," << edge.distance << " }  , ";
						d += edge.distance;
					}
					std::cout << "Минимальное расстояние - " << d << std::endl;
				}
				catch (std::exception& e)
				{
					std::cout << e.what() << std::endl;
				}
			}break;
			case 9: //задача
			{
				system("cls");
				try
				{
					Vertex v = fracture_clinic(graph);
					std::cout << "\nНаиболее удаленный от своих прямых соседей травмпункт - " << v << std::endl;
				}
				catch (std::exception& e)
				{
					std::cout << e.what() << std::endl;
				}
			}break;
		}
	}
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	Graph<int> A;
	int m = menu1();
	system("cls");
	switch (m)
	{
	case 0:
	{
		program(A);
	}break;
	case 1:
	{
		std::vector<int> vertices{ 1, 2, 3, 4, 5, 6 };
		for (int i : vertices)
			A.add_vertex(i);
		A.add_edge(1, 2, 7);
		A.add_edge(1, 3, 9);
		A.add_edge(1, 6, 14);
		A.add_edge(2, 4, 8);
		A.add_edge(3, 5, 20);
		A.add_edge(3, 6, 2);
		A.add_edge(4, 5, 10);
		A.add_edge(4, 3, 7);
		A.add_edge(5, 2, 6);
		A.add_edge(6, 5, 9);
		program(A);
	}break;
	}
}