package cat.login1st;

import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        URL url;

        try {
            url = new URL("http://web.tecnico.ulisboa.pt/ist178094/RMSF/getData.php");
            new getData().execute(url);
        } catch (MalformedURLException e) {
            e.printStackTrace();
        }
    }

    public class getData extends AsyncTask<URL, Integer, String> {

        @Override
        protected String doInBackground(URL... url) {
            HttpURLConnection urlConnection = null;


            try {
                urlConnection = (HttpURLConnection) url[0].openConnection();
                urlConnection.connect();
                InputStream in = urlConnection.getInputStream();
                InputStreamReader isr = new InputStreamReader(in);
                BufferedReader br = new BufferedReader(isr);
                StringBuilder stringB = new StringBuilder();

                String s = null;
                while ((s = br.readLine()) != null) {
                    stringB.append(s);
                }

                String r = new String();
                Pattern ptrn = Pattern.compile("\\{(.*?)\\}");
                Matcher matcher = ptrn.matcher(stringB.toString());
                while (matcher.find()) {
                    r += matcher.group(0);
                }

                JSONObject jsn = new JSONObject(r);
                String minutes = jsn.getString("minutes");
                String temperatures = jsn.getString("temperature");
                String lights = jsn.getString("light");
                String moistures = jsn.getString("moisture");
                System.out.print("moisture " + moistures);


            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                if (urlConnection != null) {
                    urlConnection.disconnect();
                }
            }
            return null;
        }

    }
}