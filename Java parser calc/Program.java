import consoleapp1.*;
import java.util.*;

public class Program
{
	public static void main(String[] args)
	{
		while (true) //Бесконечный цикл
		{
			System.out.print("Введите выражение: "); //Предлагаем ввести выражение
			System.out.println(RPN.Calculate(new Scanner(System.in).nextLine())); //Считываем, и выводим результат
		}
	}
}
