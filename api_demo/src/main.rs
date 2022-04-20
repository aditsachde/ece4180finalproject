use anyhow::Result;
use crossterm::{
    event::{self, DisableMouseCapture, EnableMouseCapture, Event, KeyCode},
    execute,
    terminal::{disable_raw_mode, enable_raw_mode, EnterAlternateScreen, LeaveAlternateScreen},
};
use std::{
    error::Error,
    io,
    ops::Range,
    time::{Duration, Instant},
};
use tui::{
    backend::{Backend, CrosstermBackend},
    layout::{Alignment, Constraint, Direction, Layout},
    style::{Color, Style},
    text::Span,
    widgets::{Block, BorderType, Borders},
    Frame, Terminal,
};
struct Data {
    roll: f32,
    pitch: f32,
    yaw: f32,
    roll_good: bool,
    pitch_good: bool,
    yaw_good: bool,
}

fn main() -> Result<(), Box<dyn Error>> {
    // setup terminal
    enable_raw_mode()?;
    let mut stdout = io::stdout();
    execute!(stdout, EnterAlternateScreen, EnableMouseCapture)?;
    let backend = CrosstermBackend::new(stdout);
    let mut terminal = Terminal::new(backend)?;

    // create app and run it
    let res = run_app(&mut terminal);

    // restore terminal
    disable_raw_mode()?;
    execute!(
        terminal.backend_mut(),
        LeaveAlternateScreen,
        DisableMouseCapture
    )?;
    terminal.show_cursor()?;

    if let Err(err) = res {
        println!("{:?}", err)
    }

    Ok(())
}

fn run_app<B: Backend>(terminal: &mut Terminal<B>) -> io::Result<()> {
    let mut last_tick = Instant::now();
    let mut data = Data {
        roll: 1.0,
        pitch: 1.0,
        yaw: 1.0,
        roll_good: true,
        pitch_good: true,
        yaw_good: true,
    };

    let tick_rate = Duration::from_millis(1000);

    loop {
        terminal.draw(|f| ui(f, &data))?;

        let timeout = tick_rate
            .checked_sub(last_tick.elapsed())
            .unwrap_or_else(|| Duration::from_secs(0));
        if crossterm::event::poll(timeout)? {
            if let Event::Key(key) = event::read()? {
                if let KeyCode::Char('q') = key.code {
                    return Ok(());
                }
            }
        }

        if last_tick.elapsed() >= tick_rate {
            let _ = update_data(&mut data);
            last_tick = Instant::now();
        }
    }
}

fn update_data(data: &mut Data) -> Result<()> {
    let client = reqwest::blocking::Client::new();
    let resp = client.post("http://192.168.1.2/").send()?.text()?;

    let values: Vec<&str> = resp.split(", ").collect();

    data.roll = values[0].parse()?;
    data.pitch = values[1].parse()?;
    data.yaw = values[2].parse()?;

    let range: Range<f32> = -30.0..30.0;

    data.roll_good = range.contains(&data.roll);
    data.pitch_good = range.contains(&data.pitch);
    data.yaw_good = range.contains(&data.yaw);

    Ok(())
}

fn ui<B: Backend>(f: &mut Frame<B>, data: &Data) {
    // Wrapping block for a group
    // Just draw the block and the group on the same area and build the group
    // with at least a margin of 1
    let size = f.size();

    // Surrounding block
    let block = Block::default()
        .borders(Borders::ALL)
        .title("AHRS Status")
        .title_alignment(Alignment::Center)
        .border_type(BorderType::Rounded);
    f.render_widget(block, size);

    let chunks = Layout::default()
        .direction(Direction::Vertical)
        .margin(4)
        .constraints(
            [
                Constraint::Percentage(33),
                Constraint::Percentage(34),
                Constraint::Percentage(33),
            ]
            .as_ref(),
        )
        .split(f.size());

    let roll_style = match data.roll_good {
        true => Style::default().bg(Color::LightGreen),
        false => Style::default().bg(Color::LightRed),
    };
    let pitch_style = match data.pitch_good {
        true => Style::default().bg(Color::LightGreen),
        false => Style::default().bg(Color::LightRed),
    };
    let yaw_style = match data.yaw_good {
        true => Style::default().bg(Color::LightGreen),
        false => Style::default().bg(Color::LightRed),
    };

    // Top left inner block with green background
    let block = Block::default()
        .title(vec![Span::styled(
            format!("Roll {}", data.roll),
            Style::default().fg(Color::Black),
        )])
        .borders(Borders::ALL)
        .style(roll_style);
    f.render_widget(block, chunks[0]);

    let block = Block::default()
        .title(vec![Span::styled(
            format!("Pitch {}", data.pitch),
            Style::default().fg(Color::Black),
        )])
        .borders(Borders::ALL)
        .style(pitch_style);
    f.render_widget(block, chunks[1]);

    let block = Block::default()
        .title(vec![Span::styled(
            format!("Yaw {}", data.yaw),
            Style::default().fg(Color::Black),
        )])
        .borders(Borders::ALL)
        .style(yaw_style);
    f.render_widget(block, chunks[2]);
}
