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
import android.widget.Spinner;

import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.components.Description;
import com.github.mikephil.charting.components.XAxis;
import com.github.mikephil.charting.components.YAxis;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.formatter.IAxisValueFormatter;
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet;

import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


public class MainActivity extends AppCompatActivity {

    int intConfig = 0;
    String email;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_main);

        Intent intent = getIntent();
        if (intent != null) {
            email = intent.getStringExtra("mail");
        }

        final Spinner config = (Spinner) findViewById(R.id.spinner);
        Button configBtn = (Button) findViewById(R.id.configBtn);
        Button refreshBtn = (Button) findViewById(R.id.refreshBtn);
        Button logoutBtn = (Button) findViewById(R.id.logoutBtn);

        URL url;

        try {
            url = new URL("http://web.tecnico.ulisboa.pt/ist178094/RMSF/getData.php");
            new getData().execute(url);
        } catch (MalformedURLException e) {
            e.printStackTrace();
        }

        refreshBtn.setOnClickListener(new View.OnClickListener() {

            /**
             * Called when the user clicks the Refresh button
             */
            @Override
            public void onClick(View view) {
                URL url;

                try {
                    url = new URL("http://web.tecnico.ulisboa.pt/ist178094/RMSF/getData.php");
                    new getData().execute(url);
                } catch (MalformedURLException e) {
                    e.printStackTrace();
                }
            }
        });



        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this,
                R.array.configs, android.R.layout.simple_spinner_item);
        // Apply the adapter to the spinner
        config.setAdapter(adapter);

        configBtn.setOnClickListener(new View.OnClickListener() {

            /**
             * Called when the user clicks the Apply button
             */
            @Override
            public void onClick(View view) {
                String itemConfig = config.getSelectedItem().toString();
                URL urlC;
                HttpURLConnection urlConnection = null;


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
                    urlC = new URL("http://web.tecnico.ulisboa.pt/ist178094/RMSF/configFromApp.php?email="
                            + email + "&configInt=" + intConfig);
                    new configSend().execute(urlC);
                } catch (MalformedURLException e) {
                    e.printStackTrace();
                }
            }

        });

        logoutBtn.setOnClickListener(new View.OnClickListener() {

            /**
             * Called when the user clicks the Logout button
             */
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(getApplicationContext(), LoginActivity.class);
                startActivity(intent);
                finish();
            }
        });


    }



    public class configSend extends AsyncTask<URL, Integer, String> {

        @Override
        protected String doInBackground(URL... url) {
            HttpURLConnection urlConnect = null;

            try {
                urlConnect = (HttpURLConnection) url[0].openConnection();
                urlConnect.setRequestMethod("GET");
                urlConnect.connect();
                InputStream in = urlConnect.getInputStream();

            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                if (urlConnect != null) {
                    urlConnect.disconnect();
                }
            }
            return null;
        }
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

            // keep zoom if screen rotated
            lineChart.setKeepPositionOnRotation(true);

            lineChart.setDragDecelerationFrictionCoef(0.9f);

            // enable scaling and dragging
            lineChart.setDragEnabled(true);
            lineChart.setScaleEnabled(true);
            lineChart.setDrawGridBackground(false);
            lineChart.setHighlightPerDragEnabled(true);
            lineChart.setBackgroundColor(Color.WHITE);

            YAxis leftAxis = lineChart.getAxisLeft();
            leftAxis.setTextColor(Color.BLACK);
            leftAxis.setDrawGridLines(true);


            YAxis rightAxis = lineChart.getAxisRight();
            rightAxis.setDrawAxisLine(false);
            rightAxis.setTextColor(Color.WHITE);
            rightAxis.setDrawGridLines(false);



            String[] dataArray = result;

            String[] date = dataArray[0].split(",");
            long[] timestamp = new long[300],  Xnew = new long[300];
            long Xold = 00L;

            for(int j = 0; j < date.length; j++) {
                timestamp[j] = Util.convertStringToTimestamp(date[j]);
                Xold = timestamp[j];
                Xnew[j] = Xold - timestamp[0];
            }

            String[] temps = dataArray[1].split(",");
            String[] light = dataArray[2].split(",");
            String[] moist = dataArray[3].split(",");

            List<Entry> yTemp = new ArrayList<Entry>();
            List<Entry> yLight = new ArrayList<Entry>();
            List<Entry> yMoist = new ArrayList<Entry>();


            /* Converting data to entries */
            for (String t : temps) {
                if(t != "")
                    yTemp.add(new Entry(Xnew[i++], Float.parseFloat(t)));
            }
            i = 0;

            for (String l : light) {
                if(l != "")
                    yLight.add(new Entry(Xnew[i++], Float.parseFloat(l)));
            }
            i = 0;

            for (String m : moist) {
                if(m != "")
                    yMoist.add(new Entry(Xnew[i++], Float.parseFloat(m)));
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

            XAxis xAxis = lineChart.getXAxis();
            xAxis.setPosition(XAxis.XAxisPosition.BOTTOM);

            IAxisValueFormatter xAxisFormatter = new HourAxisValueFormatter(timestamp[0]);
            xAxis.setValueFormatter(xAxisFormatter);

            //lineChart.moveViewTo(i, i, YAxis.AxisDependency.RIGHT);


        }
    }

}
