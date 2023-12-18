from email import encoders
from email.header import Header
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart, MIMEBase
from HTMLTable import HTMLTable
import smtplib


from dominate.tags import *


style_applied = '''
        body{
			font-family: verdana,arial,sans-serif;
			font-size:11px;
		}
		table.gridtable {
			color: #333333;
			border-width: 1px;
			border-color: #666666;
			border-collapse: collapse;
            font-size:11px;
		}
		table.gridtable th {
			border-width: 1px;
			padding: 8px;
			border-style: solid;
			border-color: #666666;
			background-color: #DDEBF7;
		}
		table.gridtable td {
			border-width: 1px;
			padding: 8px;
			border-style: solid;
			border-color: #666666;
			background-color: #ffffff;
			text-align:center;
		}
		table.gridtable td.failed {
			color:#ED5F5F;
		}
		table.gridtable td.passrate {
			font-weight:bold;
			color:green;
		}
		li {
			margin-top:5px;
		}
        div{
            margin-top:10px;
        }
    '''

def set_Hello(env, project_type):
    hello_str = "This is today's " + project_type + " API Test Report in " + env
    hello_div = div(id='hello')
    hello_div.add(p('Hi All,'))
    hello_div.add(p(hello_str))

def set_table_head():
    with tr():
        th(style='background-color:white')
        th("Passed")
        th("Failed")
        th("Total")
        th("Pass Rate")
        th("Details")

def fill_table_data(data_type, passed_count, fail_count, pass_rate, link_text, link_url):
    data_tr = tr()
    data_tr += td(data_type)
    data_tr += td(passed_count)
    data_tr += td(fail_count, cls='failed')
    data_tr += td(passed_count + fail_count)
    data_tr += td(pass_rate, cls='passrate')
    link_td = td()
    link_td += a(link_text, href=link_url)
    data_tr += link_td

def generate_result_table():
    result_div = div(id='test case result')
    with result_div.add(table(cls='gridtable')).add(tbody()):
        set_table_head()
        fill_table_data('Smoke Test Suite', 90, 10, '90%', 'Cucumber Report', 'cucumber-html-reports/overview-features.html')
        fill_table_data('Regression Test Suite', 900, 100, '90%', 'Cucumber Report','cucumber-html-reports/overview-features.html')
        fill_table_data('Summary', 990, 110, '90%', 'Pipeline Build','trigger build')

def generate_build_cause(cause):
    br()
    div(b(font('Build Information' ,color='#0B610B')))
    div(hr(size=2, alignment='center', width='100%'))
    div((b(font('Cause: Started by upstream pipeline job ' + cause))))

def generate_list_link(category, href_link):
    with li(category + ':'):
        a(href_link, href=href_link)

def generate_build_info(build_type, build_url):
    build_type_div = div()
    build_type_fond = b()
    build_type_fond += font(build_type + ' Test Build')
    build_type_div += build_type_fond
    with ul():
        generate_list_link('Build', build_url)
        generate_list_link('Cucumber Report', build_url + '/cucumber-html-reports/overview-features.html')
        generate_list_link('Log Files', build_url + '/artifact/target/rest-logs/')

def generate_ending():
    br()
    p('** This is an automatically generated email by jenkins job. **')
    p('Feel free to connect xxx-QA@xxx.com if you have any question.')

def insert_image():
    img(src='test_result_trend.png')

def generate_html_report():
    html_root = html()
    # html head
    with html_root.add(head()):
         style(style_applied, type='text/css')
    # html body
    with html_root.add(body()):
        set_Hello('develope environment', 'Project XXX')
        generate_result_table()
        generate_build_cause('Project-XXX/Dev/API')
        generate_build_info('Smoke', 'smoke build url')
        generate_build_info('Regression', 'regression build url')
        insert_image()
        generate_ending()
    # save as html file
    with open('email_report.html', 'w') as f:
        f.write(html_root.render())
    return html_root

def send_mail(context, subject):

    receiver_list = "xxxx@xx.com"
    sender = "xxxx@xxx.com"

    msg = MIMEMultipart()
    msg["From"] = sender
    msg["To"] = receiver_list
    msg["Subject"] = Header(subject, "utf-8").encode()
    msg.attach(context)
    try:
        smtpObj = smtplib.SMTP("atlsmtp10.amd.com", 25)
        smtpObj.set_debuglevel(0)
        smtpObj.starttls()
        # smtpObj.login(sender, sender_passwd)
        smtpObj.sendmail(sender, receiver_list.split(","), msg.as_string())
        print("Send out mail successfully")
        smtpObj.quit()
    except smtplib.SMTPException:
        print("Fail to send mail")

if __name__ == "__main__":
    html_context = generate_html_report()

    # print(html_context)

    subject = "test project"
    context = MIMEText( str(html_context), 'base64', 'utf-8')
    context["Content-Type"]='text/html'
    send_mail(context, subject)

