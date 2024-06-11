import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Scanner;

public class GetHtmlSource {

    public static final String ANSI_RESET = "\u001B[0m";
    public static final String ANSI_YELLOW = "\u001B[33m";
    public static final String ANSI_GREEN = "\u001B[32m";
    public static final String ANSI_CYAN = "\u001B[36m";

    public static String getHtmlSource(String urlString) {
        StringBuilder result = new StringBuilder();
        try {
            URL url = new URL(urlString);
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setRequestMethod("GET");

            int status = conn.getResponseCode();
            if (status != HttpURLConnection.HTTP_OK) {
                return "Error: " + status;
            }

            BufferedReader reader = new BufferedReader(new InputStreamReader(conn.getInputStream()));
            String line;
            while ((line = reader.readLine()) != null) {
                result.append(line).append("\n");
            }
            reader.close();
        } catch (IOException e) {
            return "Error: " + e.getMessage();
        }
        return result.toString();
    }

    public static void main(String[] args) {
        System.out.println(ANSI_YELLOW + "Welcome by github.com/c0mrade002" + ANSI_RESET);
        displayHackerEffect();

        Scanner scanner = new Scanner(System.in);
        System.out.print(ANSI_CYAN + "Inserisci l'URL del dominio (es. https://www.example.com): " + ANSI_RESET);
        String url = scanner.nextLine();
        String htmlSource = getHtmlSource(url);
        System.out.println(htmlSource);
        scanner.close();
    }

    public static void displayHackerEffect() {
        String[] hackerMessages = {
            "Accessing mainframe...",
            "Bypassing firewall...",
            "Decrypting data...",
            "Retrieving HTML source...",
            "Connection established!"
        };

        for (String message : hackerMessages) {
            System.out.println(ANSI_GREEN + message + ANSI_RESET);
            try {
                Thread.sleep(1000);  
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
    }
}