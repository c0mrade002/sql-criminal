import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.List;
import java.util.Map;
import java.util.Scanner;

public class HackerText {
    
    private static void hackerText(String text, String color) {
        String ansiColor = "";
        switch (color.toLowerCase()) {
            case "green":
                ansiColor = (char) 27 + "[32m";
                break;
            case "yellow":
                ansiColor = (char) 27 + "[33m";
                break;
            default:
                ansiColor = (char) 27 + "[0m";  // Reset color
                break;
        }
        for (char ch : text.toCharArray()) {
            System.out.print(ansiColor + ch);
            try {
                Thread.sleep(50);  // Sleep for 50 milliseconds
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                System.out.println("InterruptedException: " + e.getMessage());
            }
        }
        System.out.print((char) 27 + "[0m");  // Reset color
    }

    private static void displayCookies(String domain) {
        try {
            URL url = new URL(domain);
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            connection.connect();
            
            Map<String, List<String>> headerFields = connection.getHeaderFields();
            List<String> cookiesHeader = headerFields.get("Set-Cookie");

            if (cookiesHeader != null) {
                hackerText("\nCookies for " + domain + ":\n", "green");
                for (String cookie : cookiesHeader) {
                    hackerText(cookie + "\n", "green");
                }
            } else {
                hackerText("No cookies found for " + domain + "\n", "green");
            }

        } catch (IOException e) {
            hackerText("Error accessing " + domain + ": " + e.getMessage() + "\n", "green");
        }
    }

    public static void main(String[] args) {
        hackerText("welcome by github.com/c0mrade002\n", "yellow");
        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter the domain (e.g., http://example.com): ");
        String domain = scanner.nextLine();
        displayCookies(domain);
    }
}