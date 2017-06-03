package cat.login1st;


import android.content.Intent;
import android.graphics.Color;
import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.view.WindowManager;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.Spinner;
import android.widget.Toast;

import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.components.Description;
import com.github.mikephil.charting.components.MarkerView;
import com.github.mikephil.charting.components.XAxis;
import com.github.mikephil.charting.components.YAxis;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet;
import com.github.mikephil.charting.utils.ColorTemplate;

import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.DataOutput;
import java.io.DataOutputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static cat.login1st.R.id.text;


public class MainActivity extends AppCompatActivity {

    int intConfig = 0;
    Intent intent = getIntent();
    String email;


    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_main);

        if (intent != null) {
            email = intent.getStringExtra("mail");
        }

        URL url;

        try {
            url = new URL("http://web.tecnico.ulisboa.pt/ist178094/RMSF/getData.php");
            new getData().execute(url);
        } catch (MalformedURLException e) {
            e.printStackTrace();
        }

        final Spinner config = (Spinner) findViewById(R.id.spinner);
        Button configBtn = (Button) findViewById(R.id.configBtn);

        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this,
                R.array.configs, android.R.layout.simple_spinner_item);
// Specify the layout to use when the list of choices appears
       // adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
// Apply the adapter to the spinner
        config.setAdapter(adapter);

        configBtn.setOnClickListener(new View.OnClickListener() {

            /**
             * Called when the user clicks the Signup button
             */
            @Override
            public void onClick(View view) {
                String itemConfig = config.getSelectedItem().toString();
                URL url;

                if(itemConfig.equals("Dry")){
                    intConfig = 1;
                }
                else if(itemConfig.equals("Normal")){
                    intConfig = 2;
                }
                else if(itemConfig.equals("Moist")){
                    intConfig = 3;
                }

                try {
                    url = new URL("http://web.tecnico.ulisboa.pt/ist178094/RMSF/config.php?email="
                            + email + "&config=" + intConfig);
                } catch (MalformedURLException e) {
                    e.printStackTrace();
                }

            }

        });

    }

    public class getData extends AsyncTask<URL, Integer, String[]> {

        @Override
        protected String[] doInBackground(URL... url) {
            HttpURLConnection urlConnection = null;
            StringBuilder stringB = new StringBuilder();
            String[] respArray = new String[4];

            try {
                urlConnection = (HttpURLConnection) url[0].openConnection();
                urlConnection.connect();
                InputStream in = urlConnection.getInputStream();
                InputStreamReader isr = new InputStreamReader(in);
                BufferedReader br = new BufferedReader(isr);


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
                String min = jsn.getString("minutes");
                String temp = jsn.getString("temperature");
                String light = jsn.getString("light");
                String moist = jsn.getString("moisture");


                respArray[0] = min.replaceAll("[\\[\"\\]]", "");
                respArray[1] = temp.replaceAll("[\\[\"\\]]", "");
                respArray[2] = light.replaceAll("[\\[\"\\]]", "");
                respArray[3] = moist.replaceAll("[\\[\"\\]]", "");


            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                if (urlConnection != null) {
                    urlConnection.disconnect();
                }
            }

            return respArray;
        }

        @Override
        protected void onPostExecute(String[] result) {
            super.onPostExecute(result);
            LineChart lineChart = (LineChart) findViewById(R.id.chart);
            int i = 0;

            Description description = new Description();
            description.setText("");
            lineChart.setDescription(description);

            // enable value highlighting
            lineChart.setHighlightPerTapEnabled(true);

            // enable touch gestures
            lineChart.setTouchEnabled(true);

            lineChart.setDragDecelerationFrictionCoef(0.9f);

            // enable scaling and dragging
            lineChart.setDragEnabled(true);
            lineChart.setScaleEnabled(true);
            lineChart.setDrawGridBackground(false);
            lineChart.setHighlightPerDragEnabled(true);
            lineChart.setBackgroundColor(Color.WHITE);

            XAxis xAxis = lineChart.getXAxis();
            xAxis.setPosition(XAxis.XAxisPosition.BOTTOM);

            YAxis leftAxis = lineChart.getAxisLeft();
            leftAxis.setTextColor(Color.BLACK);
            leftAxis.setDrawGridLines(true);


            YAxis rightAxis = lineChart.getAxisRight();
            rightAxis.setDrawAxisLine(false);
            rightAxis.setTextColor(Color.WHITE);
            rightAxis.setDrawGridLines(false);



            String[] dataArray = result;

            String[] minutes = dataArray[0].split(",");
            String[] temps = dataArray[1].split(",");
            String[] light = dataArray[2].split(",");
            String[] moist = dataArray[3].split(",");

            //ArrayList<String> xTime = new ArrayList<>();
            List<Entry> yTemp = new ArrayList<Entry>();
            List<Entry> yLight = new ArrayList<Entry>();
            List<Entry> yMoist = new ArrayList<Entry>();

            /*for (String min : minutes) {
                xTime.add(new Entry(temps.getValueX(), data.getValueY()));
            }*/

            /* Converting data to entries */
            for (String t : temps) {
                yTemp.add(new Entry(i++, Float.parseFloat(t)));
            }
            i = 0;

            for (String l : light) {
                yLight.add(new Entry(i++, Float.parseFloat(l)));
            }
            i = 0;

            for (String m : moist) {
                yMoist.add(new Entry(i++, Float.parseFloat(m)));
            }

            /* Multiple data sets to multiple y axes */
            ArrayList<ILineDataSet> lineDataSets = new ArrayList<>();

            LineDataSet lineDataSet1 = new LineDataSet(yTemp,"Temperature [ºC]");
            lineDataSet1.setDrawCircles(true);
            lineDataSet1.setColor(Color.RED);

            LineDataSet lineDataSet2 = new LineDataSet(yLight,"Light [lux]");
            lineDataSet2.setDrawCircles(true);
            lineDataSet2.setColor(Color.YELLOW);

            LineDataSet lineDataSet3 = new LineDataSet(yMoist,"Moisture");
            lineDataSet3.setDrawCircles(true);
            lineDataSet3.setColor(Color.BLUE);

            lineDataSets.add(lineDataSet1);
            lineDataSets.add(lineDataSet2);
            lineDataSets.add(lineDataSet3);

            lineChart.setData(new LineData(lineDataSets));



            //lineChart.setVisibleXRangeMaximum(65f);

        }
    }


}