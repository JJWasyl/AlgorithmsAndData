#include <iostream>
#include <vector>
#include <stdexcept>
#include <stack>
#include <list>

using namespace std;

template <typename V, typename E>
class Graph{
    public:
    class Vertex{
    public:
        
        Vertex(){
            this->index = -1;
        }
        
        Vertex(const V &val, const int &ind){
            this->value = val;
            this->index = ind;

        }    
        V value;
        int index;
    };

    public:
    //edge info
    class Edge{
    public:        
        Edge(){
            this->exists = false;
        }
        
        Edge(E label){
            this->label = label;
            this->exists = true;
        }       
        bool exists;
        E label;
    };

    private:
        vector< vector<Edge> > dirmat;
        vector<Vertex> verx;
        int edgecount;
        int vertexIndex;
        
    public:
        Graph();
        Graph(const Graph &source);
        Graph(Graph &&source);
        Graph& operator=(const Graph &source);
        Graph& operator=(Graph &&source) = default;
    
    public:
        class EdgeIterator{
            friend class Graph;
            private:
                int it_row, it_col;
                Graph<V,E>* source;
                EdgeIterator(Graph<V,E> &graph, int row, int col);
                
            public:
                bool operator==(const EdgeIterator &ei) const;                
                bool operator!=(const EdgeIterator &ei) const{return !(*this == ei);}                
                EdgeIterator& operator++();                
                EdgeIterator operator++(int); 
                E& operator*();
        };
        EdgeIterator beginEdge();
        EdgeIterator endEdge();
        
    public:      
        class VertexIterator{
            friend class Graph;
            private:
                vector<Vertex> source;
                int it;
                VertexIterator(Graph<V,E> &graph, int current_vertex = 0);
            public:
                bool operator==(const VertexIterator &vi) const;
                bool operator!=(const VertexIterator &vi) const{return !(*this == vi);}
                VertexIterator& operator++();
                VertexIterator operator++(int);
                V& operator*();
        };
        VertexIterator beginVertex();
        VertexIterator endVertex();
    
    public:
        class DFS{
            friend class Graph;
            private:
                Graph<V,E>* source;
                vector<bool> visited;
                stack<int> path;
                int current_vertex_id;
                DFS(Graph<V,E> &graph, int current_vertex = 0);
                
            public:
                bool operator==(const DFS &dfs) const;
                bool operator!=(const DFS &dfs) const{return !(*this == dfs);}
                DFS& operator++();
                DFS operator++(int);
                V& operator*();
        };        
        DFS beginDFS();
        DFS endDFS();
    
    public:    
        class BFS{
            friend class Graph;
            private:
                Graph<V,E>* source;
                vector<bool> visited;
                list<int> queue;
                int current_vertex_id;
                BFS(Graph<V,E> &graph, int current_vertex = 0);
            
            public:
                bool operator==(const BFS &bfs) const;
                bool operator!=(const BFS &bfs) const{return !(*this == bfs);}
                BFS& operator++();
                BFS operator++(int);
                V& operator*();
        };        
        BFS beginBFS();
        BFS endBFS();
        
    public:
        bool addEdge(int orig, int dest, const E &label = E{}, bool replace = true);
        bool addVertex(const V &val);
        bool removeVertex(int vertIndex);
        bool removeEdge(int index1, int index2);
        bool edgeExists(int index1, int index2) const;
        int noVertices() const;
        int noEdges() const;
        void printNM();
        V& vertexData(int vertex_id);
        E& edgeLabel(int origin, int dest);

};

/*
 * Constructors
 */

template <typename V, typename E>
Graph<V,E>::Graph(){
    edgecount = 0;
    vertexIndex = 0;
}

template <typename V, typename E>
Graph<V,E>::Graph(const Graph<V,E> &source){
    dirmat = source.dirmat;
    verx = source.verx;
    edgecount = source.edgecount;
    vertexIndex = source.vertexIndex;    
}

template <typename V, typename E>
Graph<V,E>& Graph<V,E>::operator=(const Graph<V,E> &source){
    if(this == &source){
        return *this;
    }    
    dirmat = source.dirmat;
    verx = source.verx;
    edgecount = source.edgecount;
    vertexIndex = source.vertexIndex;
    return *this;
}

template <typename V, typename E>
Graph<V,E>::Graph(Graph<V,E> &&source){
    Graph();
    dirmat = source.dirmat;
    verx = source.verx;
    edgecount = source.edgecount;
    vertexIndex = source.vertexIndex;
}

/*
template <typename V, typename E>
Graph<V,E>& operator=(Graph<V,E> &&source){
    if(this == &source){
        return *this;
    }    
    dirmat = source.dirmat;
    verx = source.verx;
    edgecount = source.edgecount;
    vertexIndex = source.vertexIndex;
    return *this;
}
*/

/*
 * Main class methods
 */

template <typename V, typename E>
bool Graph<V,E>::addEdge(int orig, int dest, const E &label, bool replace){
    if(edgeExists(orig, dest)){ //rozszerzyć krawędź
        if(replace == true){
            dirmat[orig][dest] = Edge(label);
        } else {
            return false;
        }
    } else {
        dirmat[orig][dest] = Edge(label);
        
    }
    edgecount++;
    return true;
}

template <typename V, typename E>
bool Graph<V,E>::addVertex(const V &val){
    int size = verx.size();
    verx.push_back(Vertex(val, vertexIndex));
    vertexIndex++;
    size++;
    if(size > 1){
        for(int k = 0; k < size-1; k++){
            dirmat[k].push_back(Edge());
        }
    } else {
        dirmat.emplace_back();
    }
    dirmat.emplace_back();
    dirmat.back().resize(size);
    for(int k = 0; k < size; k++){
        dirmat[size].insert(dirmat[size].begin()+k, Edge());
    }
    return true;
}

//Commented methods used to reduce the Vertex vector
template <typename V, typename E>
bool Graph<V,E>::removeVertex(int vertIndex){
    int size = verx.size();
    if(size <= 0)
        return false;
    for(int k = 0; k < size; k++){
        if(verx[k].index == vertIndex){
            /*
            for(int i = size; i > vertIndex; i--){
                verx[i].index--;
            }
            */
            size--;
            //verx.erase(verx.begin() + k);
            verx[k] = Vertex();
            dirmat.erase(dirmat.begin() + vertIndex);
            for(int k = 0; k < size; k++){
                dirmat[k].erase(dirmat[k].begin() + vertIndex);
            }
            break;
        }
    }
    vertexIndex--;
    return true;
}

template <typename V, typename E>
bool Graph<V,E>::removeEdge(int index1, int index2){
    int size = dirmat[0].size();
    if(index1 > size || index2 > size)
        return false;
    
    if(edgeExists(index1, index2)){
        dirmat[index1][index2] = Edge();
        edgecount--;
        return true;
    }
    return false;
}

template <typename V, typename E>
bool Graph<V,E>::edgeExists(int index1, int index2) const{
    return dirmat[index1][index2].exists;
}

template <typename V, typename E>
int Graph<V,E>::noVertices() const{
    int temp = 0;
    for(int k = 0; k < int(verx.size()); k++){
        if(verx[k].index != -1)
            temp++;
    }
    return temp;
}

//Sprawdzic
template <typename V, typename E>
int Graph<V,E>::noEdges() const{
    return edgecount;      
}

//Multiple if statements to account for removed vertices still present as empty
//slots in the verx vector
//Technically they do nothing if the matrix was not reduced
template <typename V, typename E>
void Graph<V,E>::printNM(){
    int size = verx.size();
    cout << "__|_";
    for(int z = 0; z < size; z++){
        if(verx[z].index != -1)
            cout << verx[z].index << "_"; 
    }
    cout << endl;
    for(int k = 0; k < size; k++){
        if(verx[k].index != -1)
            cout << verx[k].index << " | ";
        for(int i = 0; i < size; i++){
            if(verx[k].index != -1 && verx[i].index != -1)
                cout << dirmat[k][i].exists << " ";
        }
        if(verx[k].index != -1)
            cout << endl;
    }   
}

/*
 * Edge Iterator Methods
 */

template <typename V, typename E>
Graph<V,E>::EdgeIterator::EdgeIterator(Graph<V,E> &graph, int row, int col){
    if(graph.edgecount == 0){
        throw invalid_argument("No edges in graph");
    } else if(row == -1 || col == -1){
        source = nullptr;
        it_row = -1;
        it_col = -1;
    } else if(row > graph.edgecount || col > graph.edgecount){
        throw invalid_argument("Values outside of graph");
    } else {
        it_row = row; it_col = col;
        while(graph.dirmat[it_row][it_col].exists != 1){
            if(it_col == graph.edgecount && it_row == graph.edgecount){
                throw invalid_argument("No edges detected");
            } else if(it_col == graph.edgecount){
                it_col = 0;
                it_row++;
            } else {
                it_col++;
            }
        }
        source = &graph;
    }
}

template <typename V, typename E>
bool Graph<V,E>::EdgeIterator::operator==(const EdgeIterator &ei) const{
    return(source == ei.source && it_row == ei.it_row && it_col == ei.it_col);
}

template <typename V, typename E>
typename Graph<V,E>::EdgeIterator& Graph<V,E>::EdgeIterator::operator++(){
    do{
        if(it_col == source->edgecount && it_row == source->edgecount){
            it_col = -1;
            it_row = -1;
            source = nullptr;
            return (*this);
        } else if(it_col == source->edgecount){
            it_col = 0;
            it_row++;
        } else {
            it_col++;
        }
    }while(source->dirmat[it_row][it_col].exists != 1);
    return(*this);
}

template <typename V, typename E>
typename Graph<V,E>::EdgeIterator Graph<V,E>::EdgeIterator::operator++(int){
    EdgeIterator temp = *this;
    ++(*this);
    return temp;
}

template <typename V, typename E>
E& Graph<V,E>::EdgeIterator::operator*(){
    return source->dirmat[it_row][it_col].label;
}

template <typename V, typename E>
typename Graph<V,E>::EdgeIterator Graph<V,E>::beginEdge(){
    return(EdgeIterator(*this, 0, 0));
}

template <typename V, typename E>
typename Graph<V,E>::EdgeIterator Graph<V,E>::endEdge(){
    return(EdgeIterator(*this, -1, -1));
}

/*
 * Vertex Iterator Methods
 */

template <typename V, typename E>
Graph<V,E>::VertexIterator::VertexIterator(Graph<V,E> &graph, int current_vertex){
    if(graph.noVertices() <= 0){
        throw invalid_argument("No existing vertices");
    } else if(current_vertex < -1 || current_vertex > graph.noVertices()){
        throw invalid_argument("Vertex does not exist");
    } else {
        source = graph.verx;
        it = current_vertex;
    }
}

template <typename V, typename E>
bool Graph<V,E>::VertexIterator::operator==(const VertexIterator &vi) const{
    return(it == vi.it);
}

template <typename V, typename E>
typename Graph<V,E>::VertexIterator& Graph<V,E>::VertexIterator::operator++(){
    do{
        ++it;
    } while(source[it].index == -1);
    return(*this);
}


template <typename V, typename E>
typename Graph<V,E>::VertexIterator Graph<V,E>::VertexIterator::operator++(int){
    VertexIterator orig = *this;
    ++(*this);
    return orig;
}


template <typename V, typename E>
V& Graph<V,E>::VertexIterator::operator*(){
    return((source[it]).value);
}

template <typename V, typename E>
typename Graph<V,E>::VertexIterator Graph<V,E>::beginVertex(){
    return VertexIterator(*this, 0);
}

template <typename V, typename E>
typename Graph<V,E>::VertexIterator Graph<V,E>::endVertex(){
    return VertexIterator(*this, int(verx.size()));
}

/*
 * DFS iterator methods
 */

template <typename V, typename E>
Graph<V,E>::DFS::DFS(Graph<V,E> &graph, int current_vertex){
    source = &graph;
    if(current_vertex < -1 || current_vertex > int(source->verx.size()))
        throw invalid_argument("Current vertex outside of range");
    
    visited.resize(int(source->verx.size()), false);
    current_vertex_id = current_vertex;
    path.push(current_vertex_id);
}

template <typename V, typename E>
bool Graph<V,E>::DFS::operator==(const DFS &dfs) const{
    return(source == dfs.source && current_vertex_id == dfs.current_vertex_id);
}

template <typename V, typename E>
typename Graph<V,E>::DFS& Graph<V,E>::DFS::operator++(){
    int s = path.top();
    path.pop();
    if(!visited[s])
        visited[s] = true;
    
    //The if statement is redundant in a non-reduced matrix
    for(int k = source->verx.size(); k >= 0; k--){
        if(source->verx[k].index != -1){
            if(source->dirmat[s][k].exists){
                if(!visited[k])
                    path.push(k);
            }
        }
    }
    if(path.empty()){
        current_vertex_id = -1;
        return(*this);
    }
    current_vertex_id = path.top();
    return(*this);
}

template <typename V, typename E>
typename Graph<V,E>::DFS Graph<V,E>::DFS::operator++(int){
    DFS orig = *this;
    ++(*this);
    return orig;
}

template <typename V, typename E>
V& Graph<V,E>::DFS::operator*(){
    return((source->verx[current_vertex_id]).value);
}

template <typename V, typename E>
typename Graph<V,E>::DFS Graph<V,E>::beginDFS(){
    return(DFS(*this, 0));
}

template <typename V, typename E>
typename Graph<V,E>::DFS Graph<V,E>::endDFS(){
    return(DFS(*this, -1));
}


/*
 * BFS iterator methods
 */
template <typename V, typename E>
Graph<V,E>::BFS::BFS(Graph<V,E> &graph, int current_vertex){
    source = &graph;
    if(current_vertex < -1 || current_vertex > int(source->verx.size()))
        throw invalid_argument("Current vertex outside of range");
    
    visited.resize(int(source->verx.size()), false);
    current_vertex_id = current_vertex;
    queue.push_back(current_vertex_id);
    if(current_vertex_id >= 0)
        visited[current_vertex_id] = true;
}

template <typename V, typename E>
bool Graph<V,E>::BFS::operator==(const BFS &bfs) const{
    return(source == bfs.source && current_vertex_id == bfs.current_vertex_id);
}

template <typename V, typename E>
typename Graph<V,E>::BFS& Graph<V,E>::BFS::operator++(){
    int s = queue.front();
    queue.pop_front();
    for(int k = 0; k < int(source->verx.size()); k++){
        if(source->verx[k].index != -1){
            if(source->dirmat[s][k].exists){
                if(!visited[k]){
                    visited[k] = true;
                    queue.push_back(k);
                }
            }
        }
    }
    if(queue.empty()){
        current_vertex_id = -1;
        return(*this);
    }
    current_vertex_id = queue.front();
    return (*this);
}

template <typename V, typename E>
typename Graph<V,E>::BFS Graph<V,E>::BFS::operator++(int){
    BFS orig = *this;
    ++(*this);
    return orig;
}

template <typename V, typename E>
V& Graph<V,E>::BFS::operator*(){
    return((source->verx[current_vertex_id]).value);
}

template <typename V, typename E>
typename Graph<V,E>::BFS Graph<V,E>::beginBFS(){
    return(BFS(*this, 0));
}

template <typename V, typename E>
typename Graph<V,E>::BFS Graph<V,E>::endBFS(){
    return(BFS(*this, -1));
}