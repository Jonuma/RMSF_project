package cat.login1st;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Patterns;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


public class LoginActivity extends AppCompatActivity {

    private static final String TAG = "LoginActivity";
    private static final int REQUEST_SIGNUP = 0;
    EditText email;
    EditText pass;
    Button loginBtn;
    TextView signupLink;
    int res = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        email = (EditText) findViewById(R.id.input_email);
        pass = (EditText) findViewById(R.id.input_password);
        loginBtn = (Button) findViewById(R.id.loginBtn);
        signupLink = (TextView) findViewById(R.id.link_signup);


        loginBtn.setOnClickListener(new View.OnClickListener() {

            /**
             * Called when the user clicks the Login button
             */
            @Override
            public void onClick(View view) {
                String user = email.getText().toString();
                String password = pass.getText().toString();

                if ((!user.equals("")) && (!password.equals(""))) {

                    if (password.length() < 5) {
                        Toast.makeText(getApplicationContext(),
                                "Password needs at least 5 characters!", Toast.LENGTH_SHORT).show();
                    }

                    if (isValidEmail(user) == true) {

                        URL url;

                        try {
                            url = new URL("http://web.tecnico.ulisboa.pt/ist178094/RMSF/loginUser.php?email=" + user +
                                    "&pass=" + password);
                            new loginCheck().execute(url);
                        } catch (MalformedURLException e) {
                            e.printStackTrace();
                        }

                    } else {
                        Toast.makeText(getApplicationContext(),
                                "Enter a valid email address!", Toast.LENGTH_SHORT).show();
                    }
                } else {
                    Toast.makeText(getApplicationContext(),
                            "One or more fields are empty!", Toast.LENGTH_SHORT).show();
                }
            }

        });

        signupLink.setOnClickListener(new View.OnClickListener() {

            /**
             * Called when the user clicks the Signup button
             */
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(getApplicationContext(), SignupActivity.class);
                startActivity(intent);
                finish();
            }

        });
    }

    public class loginCheck extends AsyncTask<URL, Integer, String> {

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
                while((s = br.readLine()) != null) {
                    stringB.append(s);
                }

                String r = new String();
                Pattern ptrn = Pattern.compile("\\{(.*?)\\}");
                Matcher matcher = ptrn.matcher(stringB.toString());
                while(matcher.find()) {
                    r += matcher.group(0);
                }

                JSONObject jsn = new JSONObject(r);
                res = jsn.getInt("success");

                System.out.print("\n\nresult " + res);


            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                if (urlConnection != null) {
                    urlConnection.disconnect();
                }
            }
        return null;
        }

       /* @Override
        protected void onProgressUpdate(Integer... progress) {
            setProgressPercent(progress[0]);
        } */

        @Override
        protected void onPostExecute(String result){

            super.onPostExecute(result);

            if(res == 0){
                Toast.makeText(getApplicationContext(),
                        "You are not registered yet. Sign up first!", Toast.LENGTH_SHORT).show();
            }else{
                Intent intent = new Intent(getApplicationContext(), MainActivity.class);
                intent.putExtra("mail", email.getText().toString());
                startActivity(intent);
                finish();
            }

        }
    }



    public final static boolean isValidEmail(CharSequence target) {
        return Patterns.EMAIL_ADDRESS.matcher(target).matches();
    }
}

