using System;
using ConsoleApp1;
class Program
{
    static void Main(string[] args)
    {
        while (true) //Бесконечный цикл
        {
            Console.Write("Введите выражение: "); //Предлагаем ввести выражение
            Console.WriteLine(RPN.Calculate(Console.ReadLine())); //Считываем, и выводим результат
        }
    }
}