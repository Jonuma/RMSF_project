package cat.login1st;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;


public class LoginActivity extends AppCompatActivity {

    private static final String TAG = "LoginActivity";
    private static final int REQUEST_SIGNUP = 0;
    EditText email;
    EditText pass;
    Button loginBtn;
    TextView signupLink;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        email = (EditText) findViewById(R.id.input_email);
        pass = (EditText) findViewById(R.id.input_password);
        loginBtn = (Button) findViewById(R.id.loginBtn);
        signupLink = (TextView) findViewById(R.id.link_signup);

       loginBtn.setOnClickListener(new View.OnClickListener() {

            /** Called when the user clicks the Login button */
            @Override
            public void onClick(View view) {
                String user = email.getText().toString();
                String password = pass.getText().toString();

                if (  ( !user.equals("")) && ( !password.equals("")) )
                {
                    if ( user.length() > 4 ){

                        URL url;

                        try {
                            url = new URL("http://web.tecnico.ulisboa.pt/ist178094/RMSF/loginUser.php?email=" + user +
                                    "&pass=" + password);
                            new loginCheck().execute(url);
                        } catch (MalformedURLException e) {
                            e.printStackTrace();
                        }

                    }
                    else
                    {
                        Toast.makeText(getApplicationContext(),
                                "Username should be minimum 5 characters", Toast.LENGTH_SHORT).show();
                    }
                }
                else
                {
                    Toast.makeText(getApplicationContext(),
                            "One or more fields are empty", Toast.LENGTH_SHORT).show();
                }
            }
           // login();


         });

        signupLink.setOnClickListener(new View.OnClickListener() {

            /** Called when the user clicks the Signup button */
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(getApplicationContext(), SignupActivity.class);
                startActivity(intent);
                finish();
            }

        });
    }

    public class loginCheck extends AsyncTask<URL, String, String> {

        @Override
        protected String doInBackground(URL... url) {
            HttpURLConnection urlConnection = null;

            try {
                urlConnection = (HttpURLConnection) url[0].openConnection();
                urlConnection.connect();
                InputStream in = urlConnection.getInputStream();
                InputStreamReader isw = new InputStreamReader(in);

                int data = isw.read();
                while (data != -1) {
                    char current = (char) data;
                    data = isw.read();
                    System.out.print(current);
                }
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

