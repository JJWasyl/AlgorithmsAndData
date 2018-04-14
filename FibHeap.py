import math
import random
import time


class FNode:
    '''klasa  zawierajaca strukture wezla'''
    def __init__(self, val, priority):
        self.val = val
        self.priority = priority
        self.parent = None
        self.child = None
        self.left = self
        self.right = self
        self.degree = 0
        self.marked = False


class FibHeap:

    def __init__(self):
        '''Konstruktor'''
        self.minimum_node = None
        self.array = []
        self.map = dict()


    def __len__(self):
        '''Zwraca wysokosc kopca'''
        return len(self.map)


    
    def add(self, val, priority):
        '''Dodaje wezel do drzewa, zwraca False jesli dana wartosc juz istnieje'''
        if val in self.map:
            return False

        wezel = FNode(val, priority)

        if self.minimum_node:
            wezel.left = self.minimum_node
            wezel.right = self.minimum_node.right
            self.minimum_node.right = wezel
            wezel.right.left = wezel

            if priority < self.minimum_node.priority:
                self.minimum_node = wezel
        else:
            self.minimum_node = wezel

        self.map[val] = wezel
        return True

    
    def decrease_key(self, val, priority):
        '''Redukuje klucz(priorytet) wezla do zadanej wartosci'''
        target_node = self.map[val]

        if not target_node:
            return False

        if target_node.priority <= priority:
            return False

        target_node.priority = priority
        y = target_node.parent
        x = target_node

        if y and x.priority < y.priority:
            self.cut(x, y)
            self.cascading_cut(y)

        if self.minimum_node.priority > x.priority:
            self.minimum_node = x

        return True

        
    def extract_minimum(self):
        '''Operacja usuwania najmniejszego wezla, wykonuje konsolidacje drzewa'''
        if len(self.map) == 0:
            raise ValueError("Pusty Kopiec")

        z = self.minimum_node
        number_of_children = z.degree
        x = z.child
        temp = None

        while number_of_children:
            temp = x.right

            x.left.right = x.right
            x.right.left = x.left

            x.left = self.minimum_node
            x.right = self.minimum_node.right
            self.minimum_node.right = x
            x.right.left = x

            x.parent = None
            x = temp
            number_of_children -= 1

        z.left.right = z.right
        z.right.left = z.left

        if z == z.right:
            self.minimum_node = None
        else:
            self.minimum_node = z.right
            self.consolidate()

        val = z.val
        del self.map[val]
        return val

            
    
    def consolidate(self):
        '''
        Operacja konsolidacji stosu, pomocnicza, 
        wykonuje rebalans stosu i zmienia jego glowne korzenie
        '''
        factor = math.log((1 + math.sqrt(5)) / 2)
        array_size = math.floor(math.log(len(self.map)) / factor) + 1

        for i in range(len(self.array)):
            self.array[i] = None

        while len(self.array) < array_size:
            self.array.append(None)
       
        x = self.minimum_node
        root_list_size = 0

        if x:
            root_list_size = 1
            x = x.right

            while x != self.minimum_node:
                root_list_size += 1
                x = x.right

        while root_list_size:
            degree = x.degree
            next = x.right

            while self.array[degree]:
                y = self.array[degree]

                if x.priority > y.priority:
                    tmp = y
                    y = x
                    x = tmp

                self.link(y, x)
                self.array[degree] = None
                degree += 1

            self.array[degree] = x
            x = next
            root_list_size -= 1

        self.minimum_node = None

        for y in self.array:
            if y:
                if not self.minimum_node:
                    self.minimum_node = y
                else:
                    y.left.right = y.right
                    y.right.left = y.left

                    y.left = self.minimum_node
                    y.right = self.minimum_node.right
                    self.minimum_node.right = y
                    y.right.left = y

                    if self.minimum_node.priority > y.priority:
                        self.minimum_node = y


    def show_roots(self):
        if len(self.map) == 0:
            raise ValueError("Pusty Kopiec")

        print "Korzenie Kopca:"
        p = self.minimum_node

        while p != None:
            print p.val,
            p = p.right

            if p != self.minimum_node:
                print "-->",
            if p == self.minimum_node:
                break
        print

            
    def link(self, y, x):
        '''Operacja wizaca korzenie, pomocnicza przy konsolidacji'''
        y.left.right = y.right
        y.right.left = y.left

        y.parent = x

        if not x.child:
            x.child = y
            y.right = y
            y.left = y
        else:
            y.left = x.child
            y.right = x.child.right
            x.child.right = y
            y.right.left = y

        x.degree += 1


    def cut(self, x, y):
        '''Opercja cut ucinajaca wezly od jednego korzenia, pomocnicza'''
        x.left.right = x.right
        x.right.left = x.left
        y.degree -= 1

        if y.child == x:
            y.child = x.right

        if y.degree == 0:
            y.child = None

        x.left = self.minimum_node
        x.right = self.minimum_node.right
        self.minimum_node.right = x
        x.right.left = x

        x.parent = None
        x.marked = False


    def cascading_cut(self, y):
        '''Operacja cascade cut wykonuje ucinanie rekurencyjnie po wszystkich drzewach'''
        z = y.parent

        if z:
            if not y.marked:
                y.marked = True
            else:
                self.cut(y, z)
                self.cascading_cut(z)


def main():


    print "\nTest 3: Oszacowanie czasu pracy operacji insert i remove min.\n"
    '''
    Test dodaje n liczb lekko zaburzonych do kopca
    Nastepnie kopiec jest oprozniany metoda delete_min
    test zwraca czas pracy obu metod dla wszystkich wartosci n
    ''' 
    
    probki = [10, 20, 50, 100, 200, 500, 1000, 10000, 50000, 100000]
    val = 0
    start_time = 0.
    elapsed_time = 0.
    heap2 = FibHeap()
    for n in probki:
        start_time = time.time()
        for i in range(n):
            val = random.randint(i, i+5)
            heap2.add(val, i)
        elapsed_time = time.time() - start_time
        print "Insert: {0} took: {1} seconds\n".format(n, elapsed_time)
    
        start_time = time.time()
        while heap2:
            heap2.extract_minimum()
        elapsed_time = time.time() - start_time
        print "Remove: {0} took: {1} seconds\n".format(n, elapsed_time)
            
            
            
    ''' TEST funkjconalny
    print "\nTest 1: Rozne liczby i klucze (zgodnosc priorytetow)\n"

    iteracje = 30
    heap = FibHeap()

    for i in range(iteracje):
        val = random.randint(0, 100)
        #prio = random.randint(0, iteracje)
        heap.add(val, val)
        print "Added (key: {0}, prio: {1})".format(val, val)


    print "\nPonizej lista korzeni kopca nieskonsolidowanego:"
    heap.show_roots()


    heap.add(5, 101)
    heap.add(450, 102)
    heap.add(965, 103)

    print "\n\nElementy 5, 450 i 965 zostaly kolejno dodane i zredukowane do klucza -1"
    print "Maja pojawic sie pierwsze jako wyniki extract_minimum:\n"

    heap.decrease_key(5, -1)
    heap.decrease_key(450, -1)
    heap.decrease_key(965, -1)

    print(heap.extract_minimum())
    print(heap.extract_minimum())
    print(heap.extract_minimum())

    print "\nKorzenie kopca po konsolidacji"
    heap.show_roots() 
 
    print "\nOperacja extract minimum oprozniajaca kopiec w sposob posortowany"

    while heap:
        print(heap.extract_minimum())
        


    print "\n\nTest 2: Identyczne klucze i priorytety, zdolnosc sortowania stosem Fibonacciego\n"

    kopiec = FibHeap()

    for i in range(iteracje):
        val = iteracje - i
        heap.add(val, val)
        print "Added (key: {0}, prio: {1})".format(val, val)

    print "\n\nOczekujemy listy posortowanej od najmniejszego do najwiekszego klucza:\n"

    while heap:
        print(heap.extract_minimum())
    print
    '''

#if __name__ == "__main__":
main()
