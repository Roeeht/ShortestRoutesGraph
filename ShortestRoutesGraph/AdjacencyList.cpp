﻿#include "AdjacencyList.h"
using namespace std;
namespace srg {

	//dtor of the graph
	void AdjacencyList::destroy()
	{
		delete[] _vertices;
		_vertices = nullptr;
		_length = 0;
	}

	/// <summary>
	/// Ctor of the graph
	/// </summary>
	/// <param name="size"></param>
	AdjacencyList::AdjacencyList(int size) : Graph(size), _vertices(nullptr)
	{
		if (size > 0)
		{
			_vertices = new Pair<int, List<Pair<int, float>>>[size];

			for (int i = 0; i < size; i++)
				_vertices[i].get_first() = i + 1;
		}
	}

	//copy ctor
	AdjacencyList::AdjacencyList(const AdjacencyList& other) : AdjacencyList(0)
	{
		*this = other;
	}

	AdjacencyList& AdjacencyList::operator=(const AdjacencyList& other)
	{
		if (this != &other)
		{
			destroy();

			if (other._length > 0)
			{
				_length = other._length;

				//already built in exception if new fails so no need to try
				_vertices = new Pair<int, List<Pair<int, float>>>[other._length];

				for (int i = 0; i < other._length; i++)
				{
					_vertices[i] = other._vertices[i];
				}
			}
		}

		return *this;
	}
	/// <summary>
	/// make an empty graph
	/// </summary>
	/// <param name="n"></param>
	/// <returns></returns>
	AdjacencyList AdjacencyList::MakeEmptyGraph(int n)
	{
		return AdjacencyList(n);
	}

	/// <summary>
	/// Returns yes if the arc (v, u) belongs to the graph, and otherwise not.
	/// </summary>
	/// <param name="u">the vertex u</param>
	/// <param name="v">the vertex v</param>
	/// <returns>true </returns>
	bool AdjacencyList::IsAdjacent(int u, int v) const
	{
		bool found = false;
		auto& adjacents = GetAdjList(u);

		for (auto itr = adjacents.begin(); itr != adjacents.end() && !found; ++itr)
			if (itr->get_first() == v)
				found = true;

		return found;
	}
	/// <summary>
	/// get vertex and return all his adjacency list
	/// </summary>
	/// <param name="u"> vertex to get his adjacency list</param>
	/// <returns> list of adjacency list </returns>
	 List<Pair<int, float>> AdjacencyList::GetAdjList(int u)
	{
		return get_adjacent_by_ref(u);
	}


	 /// <summary>
	 /// const method
	 /// gets vertex and returns all his adjacency list
	 /// </summary>
	 /// <param name="u"> vertex to get his adjacency list</param>
	 /// <returns> const list of adjacency list </returns>

	const List<Pair<int, float>> AdjacencyList::GetAdjList(int u) const
	{
		if (!check_bounds(u))
			throw std::logic_error("out of bounds - ListGraph");
		else
			return _vertices[u - 1].get_second();
	}

	/// <summary>
	/// get vertex and return all his adjacency list by reference
	/// </summary>
	/// <param name="u"> vertex to get his adjacency list</param>
	/// <returns> list of adjacency list </returns>

	List<Pair<int, float>>& AdjacencyList::get_adjacent_by_ref(int u)
	{
		if (!check_bounds(u))
			throw std::logic_error("out of bounds - ListGraph");
		else
			return _vertices[u - 1].get_second();
	}

	/// <summary>
/// get two vertices in the adjacency list and add an edge between them
/// </summary>
/// <param name="u"> source vertex </param>
/// <param name="v"> destination vertex </param>
/// <param name="weight"> edge weight </param>

	void AdjacencyList::AddEdge(int u, int v, float weight)
	{
		if (!IsAdjacent(u, v))
			get_adjacent_by_ref(u).insert_to_tail(Pair<int, float>(v, weight));
	}

	/// <summary>
/// get two vertices in the adjacency list and remove an edge between them
/// </summary>
/// <param name="u"> source vertex </param>
/// <param name="v"> destination vertex </param>

	
	void AdjacencyList::RemoveEdge(int u, int v)
	{
		auto& adjacents = get_adjacent_by_ref(u);

		for (auto itr = adjacents.begin(); itr != adjacents.end(); ++itr)
		{
			if (itr->get_first() == v)
			{
				itr.itr_delete();
				break;
			}
		}
	}

	/// <summary>
	/// create an empty adjacency list
	/// </summary>
	void AdjacencyList::makeEmpty()
	{
		for (int i = 0; i < _length; i++)
			_vertices[i].get_second().make_empty();
	}

	/*printing the graph with edges*/
			/// <summary>
		/// print adjacency list
		/// </summary>
	void AdjacencyList::PrintGraph() const
	{
		for (int i = 0; i < _length; ++i)
			if (!_vertices[i].get_second().is_empty())
			{
				std::cout << _vertices[i].get_first() << ": " << _vertices[i].get_second() << std::endl;
			}
	}

	/// <summary>
	/// Transpose of a directed graph G is another
	/// directed graph on the same set of vertices with all of the edges reversed compared to the orientation
	/// of the corresponding edges in G. That is, if G contains an edge (u, v) then the converse/transpose/reverse of G contains an edge (v, u) and vice versa.
	//Given a graph(represented as adjacency list), we need to find another graph which is the
	//transpose of the given graph.
	/// </summary>
	/// <param name="transposedGraph"></param>

		/// get an empty AdjacencyList
		/// return updated parameter as the transposed AdjacencyList
		/// </summary>
		/// <param name="transposedGraph">empty Adjacency List to update as the transposed source graph formation</param>

	void AdjacencyList::transpose(AdjacencyList* transposedGraph)
	{
		for (int i = 1; i <= this->get_length(); i++)
			for (auto j = this->get_adjacent_by_ref(i).begin(); j != this->get_adjacent_by_ref(i).end(); ++j)
			{
				transposedGraph->AddEdge(j->get_first(), i, 0);
			}
	}


	/*Breadth-first search (BFS) is an algorithm for searching a tree data structure for a node that satisfies a given property.
	 *It starts at the tree root and explores all nodes at the present depth prior to moving on to the nodes at the next depth level.
	 *Extra memory,
	 *usually a queue, is needed to keep track of the child nodes that were encountered but not yet explored.*/
	 /// <summary>
 /// get an int vertex from the adjacency list
 /// return array of distances from the parameter to each vertex
 /// </summary>
 /// <param name="s"> name of vertex</param>
 /// <returns>distances array by name of vertices</returns>
	int* AdjacencyList::BFS(int s)
	{
		int V = this->get_length();
		int* p = new int[V];
		int* d = new int[V]; //distances arr

		// Mark all the vertices as not visited
		for (int i = 0; i < V; i++)
		{
			d[i] = -1; //-1 used to initiate
			p[i] = -1;
		}
		// Create a queue for BFS
		List<int> queue;
		int j = 0; //distance from s

		// Mark the current node as visited and enqueue it
		d[s-1] = j;
		queue.insert_to_tail(s);

		// 'i' will be used to get all adjacent
		// vertices of a vertex
		List<int>::iterator i();

		while (!queue.is_empty())
		{
			// Dequeue a vertex from queue 
			int u = queue.get_head()->_item;

			queue.delete_head();
			auto adjacents = this->GetAdjList(u);
			// Get all adjacent vertices of the dequeued
			// vertex s. If a adjacent has not been visited,
			// then mark it visited and enqueue it
			for (auto v = adjacents.begin(); v != adjacents.end(); ++v)
			{
				if (d[v->get_first()-1] == -1)
				{
					d[v->get_first() - 1] = d[u - 1] + 1;
					p[v->get_first()-1] = u;
					queue.insert_to_tail(v->get_first());
				}
			}
			j++;
		}
		return d;
	}

	//A function that will build the graph from an input file, and allow reading
	//Record in the structure defined above.
	/// <summary>
		/// get from user couples of vertices and create edge between them
		/// </summary>
	void AdjacencyList::ReadGraph() 
	{
		int v, u;
		int weight = 0;
		
		while (!cin.eof())
		{
			cin >> v;
			if (check_bounds(v))
			{
				cin >> u;
				if (check_bounds(u))
					this->AddEdge(v, u, weight);
				else
				{
					cout << "invalid input";
					exit(1);
				}
			}
			else
			{
				cout << "invalid input";
				exit(1);
			}
		}


	}
}