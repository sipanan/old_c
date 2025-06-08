<?php
/**
 * Template Name: Impressum
 * 
 * @package SafeCologne
 * @since 1.0.0
 */

get_header(); ?>

<main id="main" class="page-template legal-page">
    <div class="page-hero">
        <div class="container">
            <h1><?php _e('Impressum', 'safe-cologne'); ?></h1>
            <p><?php _e('Angaben gemäß § 5 TMG', 'safe-cologne'); ?></p>
        </div>
    </div>
    
    <section class="section">
        <div class="container">
            <div class="legal-content">
                <h2><?php _e('Anbieter', 'safe-cologne'); ?></h2>
                <address>
                    <strong>Safe Cologne SecUG (haftungsbeschränkt)</strong><br>
                    Subbelrather Str. 15A<br>
                    50823 Köln<br>
                    Deutschland
                </address>
                
                <h2><?php _e('Kontakt', 'safe-cologne'); ?></h2>
                <p>
                    <strong>Telefon:</strong> <a href="tel:+4922165058801">+49 (0) 221 65058801</a><br>
                    <strong>E-Mail:</strong> <a href="mailto:info@safecologne.de">info@safecologne.de</a><br>
                    <strong>Internet:</strong> <a href="<?php echo home_url(); ?>">www.safecologne.de</a>
                </p>
                
                <h2><?php _e('Handelsregister', 'safe-cologne'); ?></h2>
                <p>
                    <strong>Registergericht:</strong> Amtsgericht Köln<br>
                    <strong>Registernummer:</strong> HRB 98765<br>
                    <strong>Umsatzsteuer-ID:</strong> DE123456789
                </p>
                
                <h2><?php _e('Geschäftsführung', 'safe-cologne'); ?></h2>
                <p>Max Mustermann</p>
                
                <h2><?php _e('Aufsichtsbehörde', 'safe-cologne'); ?></h2>
                <p>
                    Gewerbeaufsichtsamt Köln<br>
                    Willy-Brandt-Platz 2<br>
                    50679 Köln
                </p>
                
                <h2><?php _e('Berufsrechtliche Regelungen', 'safe-cologne'); ?></h2>
                <p><?php _e('Bewachungsgewerbe nach § 34a GewO', 'safe-cologne'); ?></p>
                <p><?php _e('Erlaubnis erteilt durch: Ordnungsamt der Stadt Köln', 'safe-cologne'); ?></p>
                
                <h2><?php _e('Berufshaftpflichtversicherung', 'safe-cologne'); ?></h2>
                <p>
                    <strong>Versicherungsunternehmen:</strong> Allianz Versicherungs-AG<br>
                    <strong>Versicherungsumfang:</strong> Deutschland und EU<br>
                    <strong>Deckungssumme:</strong> 2.000.000 € pro Schadensfall
                </p>
                
                <h2><?php _e('Redaktionell verantwortlich', 'safe-cologne'); ?></h2>
                <address>
                    Max Mustermann<br>
                    Subbelrather Str. 15A<br>
                    50823 Köln
                </address>
                
                <h2><?php _e('EU-Streitschlichtung', 'safe-cologne'); ?></h2>
                <p><?php _e('Die Europäische Kommission stellt eine Plattform zur Online-Streitbeilegung (OS) bereit:', 'safe-cologne'); ?> <a href="https://ec.europa.eu/consumers/odr/" target="_blank" rel="noopener">https://ec.europa.eu/consumers/odr/</a></p>
                <p><?php _e('Unsere E-Mail-Adresse finden Sie oben im Impressum.', 'safe-cologne'); ?></p>
                
                <h2><?php _e('Verbraucherstreitbeilegung/Universalschlichtungsstelle', 'safe-cologne'); ?></h2>
                <p><?php _e('Wir sind nicht bereit oder verpflichtet, an Streitbeilegungsverfahren vor einer Verbraucherschlichtungsstelle teilzunehmen.', 'safe-cologne'); ?></p>
                
                <h2><?php _e('Haftung für Inhalte', 'safe-cologne'); ?></h2>
                <p><?php _e('Als Diensteanbieter sind wir gemäß § 7 Abs.1 TMG für eigene Inhalte auf diesen Seiten nach den allgemeinen Gesetzen verantwortlich. Nach §§ 8 bis 10 TMG sind wir als Diensteanbieter jedoch nicht unter der Verpflichtung, übermittelte oder gespeicherte fremde Informationen zu überwachen oder nach Umständen zu forschen, die auf eine rechtswidrige Tätigkeit hinweisen.', 'safe-cologne'); ?></p>
                
                <h2><?php _e('Haftung für Links', 'safe-cologne'); ?></h2>
                <p><?php _e('Unser Angebot enthält Links zu externen Websites Dritter, auf deren Inhalte wir keinen Einfluss haben. Deshalb können wir für diese fremden Inhalte auch keine Gewähr übernehmen. Für die Inhalte der verlinkten Seiten ist stets der jeweilige Anbieter oder Betreiber der Seiten verantwortlich.', 'safe-cologne'); ?></p>
                
                <h2><?php _e('Urheberrecht', 'safe-cologne'); ?></h2>
                <p><?php _e('Die durch die Seitenbetreiber erstellten Inhalte und Werke auf diesen Seiten unterliegen dem deutschen Urheberrecht. Die Vervielfältigung, Bearbeitung, Verbreitung und jede Art der Verwertung außerhalb der Grenzen des Urheberrechtes bedürfen der schriftlichen Zustimmung des jeweiligen Autors bzw. Erstellers.', 'safe-cologne'); ?></p>
                
                <p class="last-updated"><?php _e('Stand:', 'safe-cologne'); ?> <?php echo date('d.m.Y'); ?></p>
            </div>
        </div>
    </section>
</main>

<?php get_footer(); ?>