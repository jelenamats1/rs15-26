#ifndef THOMSON_H
#define THOMSON_H

#include <QVector>
#include <iostream>
#include <QMap>
#include <QLinkedList>

class Gluskov;
class Deterministicki;

class Edge {

public:

    Edge();
    Edge(int state1, int state2, char c);

    int getState1() const;
    int getState2() const;
    int getC() const;

    // videti jos koliko ima smisla za ovu funkciju
    // nesto prakticnije
    bool equal1(int state, char c);

private:
    int _state1;
    int _state2;
    char _c;
};



class Automata
{

protected:
    int _start_state;
    QVector<int> _states;
    QVector<int> _accept_states;
    QVector<Edge> _edges;
    QVector<char> _alphabet;

public:
    Automata()
    {}
    Automata(int start_state);

    int getStart() const;
    QVector<int> getAcceptStates() const;
    QVector<Edge> getEdges() const;
    QVector<char> getAlphabet() const;

    void setStart(int start_state);
    void addAcceptState(int accept_state);
    void addEdge(int state1, int state2, char c);
    void addEdges(QVector<Edge> edges);
    void addChar(char c);
    void addAlphabet(QVector<char> alphabet);
    void addState(int state);

    void makeDotFile(std::ostream & osr);
};


class Thompson : public Automata
{
public:
    Thompson();
    Thompson(int first, int last);

    int getLast() const;
    Gluskov make_gluskov();

    static int state_count;
};



class Gluskov : public Automata
{ 
 //TO-DO
 // ne treba sve da bude public
public:
  Gluskov(const Thompson & t);
  Deterministicki makeDeterministicki();

private:
  QMap<int,QVector<int>> epsilon_zatvorenja;
  QLinkedList<int> kandidati;
  QVector<int> odredi_zatvorenje(int state);
  QMap<int, QVector<int>> epsilon_prelazi;
  QVector<Edge> prelazi_po_slovu;

};

class Deterministicki : public Automata
{
public:
    Deterministicki(const Gluskov & g);

    static int state_count;

private:
    QMap<int, QVector<int>> kandidati;
    QVector<QVector<int>> prelazi;
    QVector<Edge> prelazi_G;
    void dodajPrelaze(int i, const Gluskov & g);
};



#endif // THOMSON_H
