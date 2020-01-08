#include <iostream>
#include <stdexcept>
#include <boost/program_options.hpp>

#include "s2smtp/s2smtp.hpp"


int main(int argc, char* argv[])
{
    using namespace s2smtp;
    try {
        namespace po = boost::program_options;

        po::options_description desc("Simple SMTP sender based on s2smtp library "
                                     "(version " s2smtp_VERSION_STR ")\n"
                                     "Allowed options");
        desc.add_options()
            ("help,h", "Reference Information")
            ("domain,d", po::value<std::string>(), "SMTP server (e.g. smtp.gmail.com)")
            ("port,p", po::value<uint16_t>()->default_value(587), "SMTP port (standard ports are 25, 587, 465)")
            ("user,u", po::value<std::string>(), "SMTP username (correspond with email address)")
            ("password,w", po::value<std::string>()->default_value(""), "SMTP password (application or account password)")
            ("security,s", po::value<uint16_t>()->default_value(2), "Security method: 0 - none, 1 - STARTTLS, 2 - SSL/TLS")
            ("to,r", po::value<std::vector<std::string>>(), "Recipients (e.g. user_to@domain.com)")
            ("reply-to,l", po::value<std::vector<std::string>>(), "Reply to (e.g. reply@me.com)")
            ("cc,c", po::value<std::vector<std::string>>(), "Sent message copy to")
            ("bcc,b", po::value<std::vector<std::string>>(), "Sent message blind copy to")
            ("subject,t", po::value<std::string>()->default_value(""), "Message subject")
            ("text,m", po::value<std::string>()->default_value(""), "Message inline text")
            ("attach,a", po::value<std::vector<std::string>>(), "Files to send (absolute or relative path)")
            ("charset,e", po::value<std::string>()->default_value(mime::charset_t::default_charset.to_string()), "Input character encoding")
        ;

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);    

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return 0;
        }

        if (!vm.count("domain")) throw std::domain_error("Domain was not set.");
        if (vm["security"].as<uint16_t>() > 2) throw std::domain_error("Incorrect security option.");
        if (!vm.count("user")) throw std::domain_error("User was not set.");
        if (!vm.count("to")) throw std::domain_error("Recipients were not set.");

        end_point_t ep{ vm["domain"].as<std::string>(), vm["port"].as<uint16_t>() };        
        auto sm = static_cast<security_method_t>(vm["security"].as<uint16_t>());    
        credentials_t cred{ vm["user"].as<std::string>(), vm["password"].as<std::string>() };

        mime::charset_t::default_charset = mime::charset_t(vm["charset"].as<std::string>());
        mime::message_t msg(cred.user);        
        
        auto populate_rcpts = [] (const std::vector<std::string>& arg_list,
                                    mime::address_list_t& target_list)
        {
            std::transform(arg_list.begin(), arg_list.end(), std::back_inserter(target_list),
                   [](const std::string& a) -> mime::address_t { return mime::mailbox_t(a); });
        };

        populate_rcpts(vm["to"].as<std::vector<std::string>>(), msg.header.to);

        if (vm.count("cc"))
            populate_rcpts(vm["cc"].as<std::vector<std::string>>(), msg.header.cc);
        if (vm.count("bcc"))
            populate_rcpts(vm["bcc"].as<std::vector<std::string>>(), msg.header.bcc);
        if (vm.count("reply-to"))
            populate_rcpts(vm["reply-to"].as<std::vector<std::string>>(), msg.header.reply_to); 

        msg.header.subject = vm["subject"].as<std::string>();
        msg.text.content = vm["text"].as<std::string>();

        if (vm.count("attach"))
        {
            for (const auto& file_path: vm["attach"].as<std::vector<std::string>>())
            {
                msg.attachments.push_back(mime::attachment_t(file_path));
            }
        }
        
        std::cout << "connecting to " << ep.domain << "\n";
        client_t client(ep, sm, cred);
        std::cout << "initiate a mail transaction\n";
        client.submit(msg);
        std::cout << "mail transferred\n";
        return 0;

    } 
    catch(mime_error& e) {
        std::cout << "message composing error\ndetails: " << e.what() << std::endl;
        return 1;
    } catch(network_error& e) {
        std::cout << "network error\ndetails: " << e.what() << std::endl;
        return 1;
    } catch(smtp_error& e) {
        std::cout << "smtp session error\ndetails: " << e.what() << std::endl;
        return 1;
    } catch(std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}
